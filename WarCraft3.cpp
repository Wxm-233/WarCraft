#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class Weapon;
class City;
class HeadQuarter;
class Warrior;

int WarriorElements[5] = {0};
int WarriorForces[5] = {0};

int M, N, K, T, Time;

enum WarriorType {dragon = 0, ninja, iceman, lion, wolf};
enum camp {red = 0, blue};

void printTime() {
    printf("%03d:%02d ", Time / 60, Time % 60);
}
static bool cmpWeapons(const Weapon* w1, const Weapon* w2);

class Weapon {
private:
    int durability;
public:
    enum type {sword = 0, bomb, arrow};
    type t;
    Weapon(type t) : t(t) {
        switch (t) {
            case sword: durability = -1; break;
            case bomb: durability = 1; break;
            case arrow: durability = 2; break;
            default: break;
        }
    }
    bool attack(int f, int& userE, int& attackedE, bool isNinja) {
        switch (t) {
            case sword: attackedE -= force(f); break;
            case bomb: attackedE -= force(f), userE -= (isNinja ? 0 : force(f) / 2), --durability; break;
            case arrow: attackedE -= force(f), --durability; break;
        }
        return durability == 0;
    }
    int force(int f) {
        switch (t) {
            case sword: return f / 5;
            case bomb: return f * 2 / 5;
            case arrow: return f * 3 / 10;
            default: return 0;
        }
    }
    std::string WeaponType() {
        switch (t) {
            case sword: return "sword";
            case bomb: return "bomb";
            case arrow: return "arrow";
            default: return "errorType";
        }
    }
    bool operator<(const Weapon* another) const {
        if (this->t == arrow && another->t == arrow) {
            return this->durability < another->durability;
        }
        else return this->t < another->t;
    }
    bool operator>(const Weapon& another) {
        if (this->t == arrow && another.t == arrow) {
            return this->durability > another.durability;
        }
        else return this->t > another.t;
    }
    friend bool cmpWeapons(const Weapon* w1, const Weapon* w2);
};

class City {
protected:
    const int number;
public:
    City(int number) : number(number), RedW(nullptr), BlueW(nullptr) {

    }
    Warrior* RedW = nullptr;
    Warrior* BlueW = nullptr;
    bool fight();
    void lion_check();
    void wolf_check();
    void fightingResult_output();
    virtual bool move_output();
};

class HeadQuarter : public City {
using wt = WarriorType;
private:
    camp c;
    int element;
    wt s[5];//Spawn Sequence
    int WarriorID = 1;
    bool isSpawning = true;
public:
    HeadQuarter(camp c, int element, wt s1, wt s2, wt s3, wt s4, wt s5, int number) : c(c), element(element), City(number) {
        s[0] = s1, s[1] = s2, s[2] = s3, s[3] = s4, s[4] = s5;
    }
    std::string cityCamp() {
        switch (c) {
            case red: return "red";
            case blue: return "blue";
            default: return "errorType";
        }
    }
    bool spawn();
    bool move_output();
    void report() {
        printTime();
        std::cout << element << " elements in " << cityCamp() << " headquarter" << std::endl;
    }
};

class Warrior {
protected:
    int position;
    camp c;
    int n_weapons[3] = {0};
    void count_weapons() {
        n_weapons[0] = n_weapons[1] = n_weapons[2] = 0;
        for (auto i = Weapons.begin(); i < Weapons.end(); ++i) {
            ++n_weapons[(*i)->t];
        }
    }
public:
    int force;
    int element;
    const int number;
    WarriorType type;
    std::vector<Weapon*> Weapons;
    std::vector<Weapon*>::iterator currentWeapon;
    Warrior(int number, WarriorType wt, camp c) : element(WarriorElements[wt]), force(WarriorForces[wt]), number(number), type(wt), c(c) {
        currentWeapon = Weapons.begin();
        switch (c) {
            case red: position = 0; break;
            case blue: position = N + 1;break;
            default: position = 0;break;
        }
    }

    std::string camptype() {
        switch (c) {
            case red: return "red";
            case blue: return "blue";
            default: return "errorType";
        }
    }

    std::string name() {
        switch (type) {
            case dragon: return "dragon";
            case ninja: return "ninja";
            case iceman: return "iceman";
            case lion: return "lion";
            case wolf: return "wolf";
            default: return "errorType";
        }
    }

    bool isDead() {
        return element <= 0;
    }

    void sortWeapons() {
        if (Weapons.empty()) return;
        std::sort(Weapons.begin(), Weapons.end(), cmpWeapons);
        //there already has a sort function，but it's not what I want. However, it works.
        currentWeapon = Weapons.begin();
    }

    void attack(Warrior& another, bool status_Another) {
        if (this->isDead() || another.isDead())
            return;//when either is dead the battle is over

        if (Weapons.empty()) {
            if (status_Another)
                another.attack(*this, false);
            else return;
        }

        else if ((*(Weapons.begin()))->t == Weapon::sword && (*(Weapons.rbegin()))->t == Weapon::sword && (*(Weapons.begin()))->force(force) == 0)
            if (status_Another)
                another.attack(*this, false);
            else return;
        //in the two situations above, no harm will be made, and no consumption of weapons can be caused by *this,
        //in other words, status_This == false. So if (status_Another == false), the battle will end

        else {//some damage to either another or the weapons will be made

            //the current weapon gets broken
            if ((*currentWeapon)->attack(force, element, another.element, this->type == ninja)) {
                delete *currentWeapon;
                Weapons.erase(currentWeapon);//it disappears
                if (Weapons.empty()) {//status_This == false
                    if (status_Another)
                        another.attack(*this, false);//no weapons after that
                    else return;
                }
                else if (currentWeapon < Weapons.end()) {//no need to move, currentWeapon is already the next one
                    if (status_Another)
                        another.attack(*this, true);
                    else this->attack(another, false);
                }
                else {
                    currentWeapon = Weapons.begin();//go back to the first one
                    if (status_Another)
                        another.attack(*this, true);//no weapons after that
                    else this->attack(another, false);
                }
                return;//this if-statement has ended
            }

            //the current weapon is not broken
            if (currentWeapon + 1 < Weapons.end()) {//has not reached the last one
                ++currentWeapon;
                if (status_Another)
                    another.attack(*this, true);
                else this->attack(another, false);
            } else {
                currentWeapon = Weapons.begin();//reached, return to the first one
                if (status_Another)
                    another.attack(*this, true);
                else this->attack(another, false);
            }
        }
    }

    void capture(Warrior& another) {
        another.sortWeapons();
        for (auto i = another.Weapons.begin(); i < another.Weapons.end(); ) {
            if (this->Weapons.size() >= 10)
                return;
            this->Weapons.push_back(*i);
            another.Weapons.erase(i);
        }
    }

    void report() {
        count_weapons();
        printTime();
        std::cout << camptype() << ' ' << name() << ' ' << number << " has "
        << n_weapons[Weapon::sword] << " sword "
        << n_weapons[Weapon::bomb] << " bomb "
        << n_weapons[Weapon::arrow] << " arrow and " << element << " elements" << std::endl;
    }

    virtual void move(int step) {
        position += step;
    }//the step is positive when moving from west to east, or from red to blue
};

class Dragon : public Warrior {
public:
    Dragon(int number, camp c) : Warrior(number, dragon, c) {
        Weapons.push_back(new Weapon(Weapon::type(number % 3)));
    }
    std::string yell() {
        return camptype() + " dragon " + std::to_string(number) + " yelled in city " + std::to_string(position); 
    }//e.g. blue dragon 2 yelled in city 4
};

class Ninja : public Warrior {
public:
    Ninja(int number, camp c) : Warrior(number, ninja, c) {
        Weapons.push_back(new Weapon(Weapon::type(number % 3)));
        Weapons.push_back(new Weapon(Weapon::type((number+1) % 3)));
    }
};

class Iceman : public Warrior {
public:
    Iceman(int number, camp c) : Warrior(number, iceman, c) {
        Weapons.push_back(new Weapon(Weapon::type(number % 3)));
    }
    void move(int step) {
        Warrior::move(step);
        element -= element / 10;//loses elements when moving
    }
};

class Lion : public Warrior {
private:
    int loyalty;
public:
    Lion(int number, camp c, int loyalty) : Warrior(number, lion, c), loyalty(loyalty) {
        Weapons.push_back(new Weapon(Weapon::type(number % 3)));
    }
    bool isLoyal() {
        return loyalty > 0;
    }
    std::string runAwayInfo() {
        return camptype() + " lion " + std::to_string(number) + " ran away";
        //e.g. red lion 4 ran away
    }
    void move(int step) {
        Warrior::move(step);
        loyalty -= K;
    }
};

class Wolf : public Warrior {
public:
    Wolf(int number, camp c) : Warrior(number, wolf, c) {

    }
    void rob(Warrior& another) {
        if (another.Weapons.empty() || another.type == wolf)
            return;
        another.sortWeapons();
        Weapon::type robbedType = (*(another.Weapons.begin()))->t;
        int robbedNum = 0;
        for (auto i = another.Weapons.begin(); i < another.Weapons.end(); ) {//the iterator need not be moved
            if ((*i)->t != robbedType || this->Weapons.size() >= 10)
                break;
            this->Weapons.push_back(*i);
            another.Weapons.erase(i);
            ++robbedNum;
        }
        printTime();
        std::cout << camptype() << " wolf " << number << " took " << robbedNum << ' ' 
        << (*(this->Weapons.rbegin()))->WeaponType() << " from " << another.camptype() << ' ' 
        << another.name() << ' ' << another.number << " in city " << this->position << std::endl;
        //e.g. red wolf 3 took 2 arrow from blue lion 2 in city 5
    }
};

bool City::fight() {
    if (!(RedW && BlueW))
        return false;//if red or blue does not exist, the fight will not begin
    RedW->sortWeapons();
    RedW->currentWeapon = RedW->Weapons.begin();
    BlueW->sortWeapons();
    BlueW->currentWeapon = BlueW->Weapons.begin();
    if (number % 2) {
        RedW->attack(*BlueW, true);//red first in odd city
    } else {
        BlueW->attack(*RedW, true);//blue first in even city
    }
    return true;
}

void City::lion_check() {
    if (RedW && RedW->type == lion && !((Lion*)(RedW))->isLoyal()) {
        printTime();
        std::cout << ((Lion*)(RedW))->runAwayInfo() << std::endl;
        delete RedW;
        RedW = nullptr;
    }
    if (BlueW && BlueW->type == lion && !((Lion*)(BlueW))->isLoyal()) {
        printTime();
        std::cout << ((Lion*)(BlueW))->runAwayInfo() << std::endl;
        delete BlueW;
        BlueW = nullptr;
    }
}

void City::wolf_check() {
    if (RedW && RedW->type == wolf && BlueW) {
        ((Wolf*)(RedW))->rob(*BlueW);
    }
    if (BlueW && BlueW->type == wolf && RedW) {
        ((Wolf*)(BlueW))->rob(*RedW);
    }
}

bool City::move_output() {
    if (RedW) {
        printTime();
        std::cout << "red " << RedW->name() << ' ' << RedW->number <<
        " marched to city " << number << " with " << RedW->element <<
        " elements and force " << RedW->force << std::endl;
        //e.g red iceman 1 marched to city 1 with 20 elements and force 30
    }
    if (BlueW) {
        printTime();
        std::cout << "blue " << BlueW->name() << ' ' << BlueW->number <<
        " marched to city " << number << " with " << BlueW->element <<
        " elements and force " << BlueW->force << std::endl;
        //e.g blue iceman 1 marched to city 1 with 20 elements and force 30
    }
    return false;
}

void City::fightingResult_output() {
    printTime();

    if (RedW->isDead() && BlueW->isDead()) {
        std::cout << "both red " << RedW->name() << ' ' << RedW->number <<
        " and blue " << BlueW->name() << ' ' << BlueW->number << " died in city " << number << std::endl; 
        //e.g. both red iceman 1 and blue lion 12 died in city 2
        delete BlueW, delete RedW;
        BlueW = RedW = nullptr;
    } else if (!RedW->isDead() && BlueW->isDead()) {
        std::cout << "red " << RedW->name() << ' ' << RedW->number << " killed blue "
        << BlueW->name() << ' ' << BlueW->number << " in city " 
        << number << " remaining " << RedW->element << " elements" << std::endl;
        //e.g. red iceman 1 killed blue lion 12 in city 2 remaining 20 elements
        RedW->capture(*BlueW);
        delete BlueW;
        BlueW = nullptr;
    } else if (RedW->isDead() && !BlueW->isDead()) {
        std::cout << "blue " << BlueW->name() << ' ' << BlueW->number << " killed red "
        << RedW->name() << ' ' << RedW->number << " in city "
        << number << " remaining " << BlueW->element << " elements" << std::endl;
        //e.g. blue iceman 1 killed red lion 6 in city 3 remaining 15 elements
        BlueW->capture(*RedW);
        delete RedW;
        RedW = nullptr;
    } else {
        std::cout << "both red " << RedW->name() << ' ' << RedW->number << 
        " and blue " << BlueW->name() << ' ' << BlueW->number << " were alive in city " << number << std::endl; 
        //e.g. both red iceman 1 and blue lion 12 were alive in city 2
    }
    if (RedW && RedW->type == dragon) {
        printTime();
        std::cout << "red dragon " << RedW->number << " yelled in city " << number << std::endl;
    }
    if (BlueW && BlueW->type == dragon) {
        printTime();
        std::cout << "blue dragon " << BlueW->number << " yelled in city " << number << std::endl;
    }
}

bool HeadQuarter::spawn() {
    WarriorType t = s[(WarriorID-1) % 5];
    if (element < WarriorElements[t])
        return false;
    element -= WarriorElements[t];
    Warrior* tmpW = nullptr;
    switch (t) {
        case dragon: tmpW = new Dragon(WarriorID, c); break;
        case ninja: tmpW = new Ninja(WarriorID, c); break;
        case iceman: tmpW = new Iceman(WarriorID, c); break;
        case lion: tmpW = new Lion(WarriorID, c, element); break;
        case wolf: tmpW = new Wolf(WarriorID, c); break;
    }
    printTime();
    std::cout << cityCamp() << ' ' << tmpW->name() << ' ' << WarriorID << " born" << std::endl;
    ++WarriorID;
    if (t == lion) {
        std::cout << "Its loyalty is " << element << std::endl;
    }
    switch (c) {
        case red: RedW = tmpW; break;
        case blue: BlueW = tmpW; break;
        default: break;
    }
    return true;
}

bool HeadQuarter::move_output() {
    if (c == blue && RedW) {
        printTime();
        std::cout << "red " << RedW->name() << ' ' << RedW->number <<
        " reached blue headquarter with " << RedW->element <<
        " elements and force " << RedW->force << std::endl;
        printTime();
        std::cout << "blue headquarter was taken" << std::endl;
        return true;
    }
    if (c == red && BlueW) {
        printTime();
        std::cout << "blue " << BlueW->name() << ' ' << BlueW->number <<
        " reached red headquarter with " << BlueW->element <<
        " elements and force " << BlueW->force << std::endl;
        printTime();
        std::cout << "red headquarter was taken" << std::endl;
        return true;
    }
    return false;
}

static bool cmpWeapons(const Weapon* w1, const Weapon* w2) {
    if (w1->t == Weapon::arrow && w2->t == Weapon::arrow) {
            return w1->durability < w2->durability;
        }
        else return w1->t < w2->t;
}

int main() {
    int nCase;
    std::cin >> nCase;
    for (int Case = 1; Case <= nCase; ++Case) {
        std::cout << "Case " << Case << ":" << std::endl;
        std::cin >> M >> N >> K >> T;
        std::cin >> WarriorElements[0] >> WarriorElements[1] >> WarriorElements[2] >> WarriorElements[3] >> WarriorElements[4];
        std::cin >> WarriorForces[0] >> WarriorForces[1] >> WarriorForces[2] >> WarriorForces[3] >> WarriorForces[4];
        HeadQuarter* redHQ = new HeadQuarter(red, M, iceman, lion, wolf, ninja, dragon, 0);
        HeadQuarter* blueHQ = new HeadQuarter(blue, M, lion, dragon, ninja, iceman, wolf, N + 1);
        std::vector<City*> Cities;
        Cities.push_back(redHQ);
        for (int i = 1; i <= N; ++i) {
            Cities.push_back(new City(i));
        }
        Cities.push_back(blueHQ);
        for (Time = 0; Time <= T; ) {
            //:00
            redHQ->spawn(), blueHQ->spawn();//spawn warriors
            Time += 5;
            if (Time > T) break;

            //:05
            for (auto i = Cities.begin(); i < Cities.end(); ++i) {
                (*i)->lion_check();
            }//check the lions to run
            Time += 5;
            if (Time > T) break;

            //:10
            for (auto i = Cities.begin() + 1; i < Cities.end(); ++i) {
                if ((*i)->BlueW){
                    (*i)->BlueW->move(-1);
                    (*(i-1))->BlueW = (*i)->BlueW;
                    (*i)->BlueW = nullptr;
                }
            }
            for (auto i = Cities.rbegin() + 1; i < Cities.rend(); ++i) {
                if ((*i)->RedW) {
                    (*i)->RedW->move(1);
                    (*(i-1))->RedW = (*i)->RedW;
                    (*i)->RedW = nullptr;
                }
            }
            bool do_endLoop = false;
            for (auto i = Cities.begin(); i < Cities.end(); ++i) {
                do_endLoop = (*i)->move_output() || do_endLoop;//output all moving information in these cities
            }
            if (do_endLoop)
                break;//if one headquarter is taken then the current game is over
            Time += 25;
            if (Time > T) break;

            //:35
            for (auto i = Cities.begin(); i < Cities.end(); ++i) {
                (*i)->wolf_check();
            }//check the wolves to rob
            Time += 5;
            if (Time > T) break;

            //:40
            for (auto i = Cities.begin(); i < Cities.end(); ++i) {
                if ((*i)->fight()) {
                    (*i)->fightingResult_output();
                }
            }//every city with both warriors begins a fight
            Time += 10;
            if (Time > T) break;

            //:50
            redHQ->report(), blueHQ->report();
            //headquarters reporting their situations
            Time += 5;
            if (Time > T) break;

            //:55
            for (auto i = Cities.begin(); i < Cities.end(); ++i) {
                if ((*i)->RedW) {
                    (*i)->RedW->report();
                }
                if ((*i)->BlueW) {
                    (*i)->BlueW->report();
                }
            }//existing warriors reporting
            Time += 5;
        }
    }
    return 0;
}
