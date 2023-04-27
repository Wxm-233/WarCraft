#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <iomanip>

using std::string;
using std::cin;
using std::cout;
using std::vector;

class Warrior;
class Weapon;
class City;
class HeadQ;
class Ninja;
class Dragon;
class Iceman;
class Lion;
class Wolf;

enum WarriorType {
    dragon, ninja, iceman, lion, wolf
};

enum WeaponType {
    sword, bomb, arrow
};

enum Camp {
    none, red, blue
};

int InitElements[5];
int InitForces[5];

int M, N, R, K, T;
int Time;

int PrintTime() {
    return printf("%03d:%02d ", Time/60, Time%60);
}

class Weapon {
private:
    WeaponType Type;
public:
    Weapon(WeaponType Type) : Type(Type) {}
};

class Sword : public Weapon {
private:
    int force_;
public:
    Sword(int force) : Weapon(sword), force_(force) {}
    bool getForce(bool isReal, int& result) {//returns if this sword is broken
        result = force_;
        if (isReal) {
            force_ = force_ * 4 / 5;
            if (force_ == 0)
                return true;
        }
        return false;
    }
    int force() {
        return force_;
    }
};

class Bomb : public Weapon {
public:
    Bomb() : Weapon(bomb) {}
};

class Arrow : public Weapon {
private:
    int durability = 3;
public:
    Arrow() : Weapon(arrow) {}
    bool use() {//returns does_not_exist()
        --durability;
        return durability == 0;
    }
    int getD() {
        return durability;
    }
};

class Warrior {
protected:
    WarriorType Type_;
    Camp camp_;
    const int number_;
    int elements_;
    int force;
    int location;
    HeadQ& HQ;
    void getWeaponByNumber(int number) {
        switch (WeaponType(number)) {
            case bomb: wBomb = new Bomb(); break;
            case sword: wSword = force / 5 == 0 ? nullptr : new Sword(force / 5); break;
            case arrow: wArrow = new Arrow(); break;
            default: break;
        }
    }
    void getWeapon() {
        switch (Type_) {
            case dragon:
            case iceman:
                getWeaponByNumber(number_%3);
                break;
            case ninja:
                getWeaponByNumber(number_%3);
                getWeaponByNumber((number_+1)%3);
                break;
            default:
                break;
        }
    }
public:
    Sword* wSword = nullptr;
    Bomb* wBomb = nullptr;
    Arrow* wArrow = nullptr;
    Warrior(WarriorType Type, int number, int elements, int force, HeadQ& HQ);
    bool isAlive() {
        return elements_ > 0;
    }
    void add_elements(int gain) {
        elements_ += gain;
    }
    void get_elements_for_HQ(int gain);
    void get_rewards_from_HQ();
    void attack(Warrior& another, bool isReal) {
        if (!isAlive() || !another.isAlive()) {
            return;
        }
        int force_of_weapon = 0;
        if (wSword != nullptr) {
            if (wSword->getForce(isReal, force_of_weapon)) {
                delete wSword;
                wSword = nullptr;
            }
        }
        another.elements_ -= (force + force_of_weapon);
        if (isReal) {
            PrintTime();
            printf("%s %s %d attacked %s %s %d in city %d with %d elements and force %d\n",
            getCamp(), getName(), number_, another.getCamp(), another.getName(), another.number_, location, elements_, force);
        }
        if (another.isAlive() || !isReal) {
            another.fight_back(*this, isReal);
        }
    }
    virtual void fight_back(Warrior& another, bool isReal) {
        int force_of_weapon = 0;
        if (wSword != nullptr) {
            if (wSword->getForce(isReal, force_of_weapon)) {
                delete wSword;
                wSword = nullptr;
            }
        }
        another.elements_ -= (force/2 + force_of_weapon);
        if (isReal) {
            PrintTime();
            printf("%s %s %d fought back against %s %s %d in city %d\n",
            getCamp(), getName(), number_, another.getCamp(), another.getName(), another.number_, location);
        }
        
    }
    bool mock_fight(Warrior& another, Camp flag, int number) {//if returns true the warrior will use a bomb
        Camp first = (flag == red || (flag == none && number % 2)) ? red : blue;
        return camp_ == first ? (another.Type_ != ninja && force + (wSword == nullptr ? 0 : wSword->force()) < another.elements_ && another.force / 2 + (another.wSword == nullptr ? 0 : another.wSword->force()) > elements_) :
        (another.force + (another.wSword == nullptr ? 0 : another.wSword->force()) >= elements_);
    }
    bool useBomb(Warrior* another, Camp flag, int number) {
        if (wBomb != nullptr && another != nullptr && mock_fight(*another, flag, number) && isAlive() && another->isAlive()) {
            PrintTime();
            printf("%s %s %d used a bomb and killed %s %s %d\n", getCamp(), getName(), number_, another->getCamp(), another->getName(), another->number_);
            return true;
        }
        return false;
    }
    virtual void move(int step) {
        location += step;
    }
    void shoot(Warrior* another) {
        if (wArrow != nullptr && another != nullptr) {
            if (wArrow->use()) {
                delete wArrow;
                wArrow = nullptr;
            }
            another->elements_ -= R;
            PrintTime();
            printf("%s %s %d shot", getCamp(), getName(), number_);
            if (another->isAlive()) {
                printf("\n");
            } else {
                printf(" and killed %s %s %d\n", another->getCamp(), another->getName(), another->number_);
            }
        }
    };
    const char* getCamp() {
        switch (camp_) {
            case red: return "red";
            case blue: return "blue";
            default: return "error";
        }
    }
    const char* getName() {
        switch (Type_) {
            case ninja: return "ninja";
            case dragon: return "dragon";
            case iceman: return "iceman";
            case lion: return "lion";
            case wolf: return "wolf";
            default: return "error";
        }
    }

    void print_death_info() {
        PrintTime();
        printf("%s %s %d was killed in city %d\n", getCamp(), getName(), number_, location);
    }

    void print_earning_info(int element) {
        PrintTime();
        printf("%s %s %d earned %d elements for his headquarter\n", getCamp(), getName(), number_, element);
    }

    void reportWeapons() {
        PrintTime();
        printf("%s %s %d has ", getCamp(), getName(), number_);
        if (wArrow == nullptr && wBomb == nullptr && wSword == nullptr)
            printf("no weapon\n");
        else {
            bool isFirst = true;
            if (wArrow != nullptr) {
                printf("arrow(%d)", wArrow->getD());
                isFirst = false;
            }
            if (wBomb != nullptr) {
                if (!isFirst)
                    printf(",");
                printf("bomb");
                isFirst = false;
            }
            if (wSword != nullptr) {
                if (!isFirst)
                    printf(",");
                printf("sword(%d)", wSword->force());
            }
            printf("\n");
        }
    }

    void output_reaching_information() {
        PrintTime();
        if (location == 0 && camp_ == blue) {
            printf("blue %s %d reached red headquarter with %d elements and force %d\n", getName(), number_, elements_, force);
        } else if (location == N+1 && camp_ == red) {
            printf("red %s %d reached blue headquarter with %d elements and force %d\n", getName(), number_, elements_, force);
        } else {
            printf("%s %s %d marched to city %d with %d elements and force %d\n", getCamp(), getName(), number_, location, elements_, force);
        }
    }
    WarriorType Type() {
        return Type_;
    }
    Camp camp() {
        return camp_;
    }
    int elements() {
        return elements_;
    }
    int number() {
        return number_;
    }
};

class Ninja : public Warrior {
public:
    void fight_back(Warrior& another, bool isReal) {
        //do nothing
    }
    Ninja(int number, int elements, int force, HeadQ& HQ) : Warrior(ninja, number, elements, force, HQ) {}
};

class Dragon : public Warrior {
private:
    double morale;
public:
    Dragon(int number, int elements, int force, HeadQ& HQ);
    void morale_change(bool win) {
        morale += win ? 0.2 : -0.2;
    }
    void yell() {
        if (morale > 0.8) {
            PrintTime();
            printf("%s dragon %d yelled in city %d\n", getCamp(), number_, location);
        }
    }
};

class Iceman : public Warrior {
private:
    int steps = 0;
public:
    Iceman(int number, int elements, int force, HeadQ& HQ) : Warrior(iceman, number, elements, force, HQ) {}
    void move(int step) {
        Warrior::move(step);
        ++steps;
        if ((steps % 2) == 0) {
            elements_ = elements_ - 9 > 0 ? elements_ - 9 : 1;
            force += 20;
        }
    }
};

class Lion : public Warrior {
private:
    int loyalty;
public:
    Lion(int number, int elements, int force, HeadQ& HQ);
    bool is_to_run_away() {
        return loyalty <= 0 && PrintTime() && printf("%s lion %d ran away\n", getCamp(), number_);
    }
    int elements_before_fighting;
    void loyaltyLose() {
        loyalty -= K;
    }
};

class Wolf : public Warrior {
public:
    Wolf(int number, int elements, int force, HeadQ& HQ) : Warrior(wolf, number, elements, force, HQ) {}
    void grab_Weapon(Warrior& another) {
        if (this->wArrow == nullptr)
            this->wArrow = another.wArrow;
        if (this->wBomb == nullptr)
            this->wBomb = another.wBomb;
        if (this->wSword == nullptr)
            this->wSword = another.wSword;
    }
};

class City {
protected:
    int elements_;
    Camp last_result = none;
    bool flag_Raised = false;
public:
    Camp flag = none;
    const int number;
    bool two_are_both_alive_before = false;
    Warrior* RedWarrior = nullptr;
    Warrior* BlueWarrior = nullptr;
    Warrior* Won_Warrior = nullptr;
    City(int number, int elements) : number(number), elements_(elements) {

    }
    int elements() {
        return elements_;
    }
    void produce() {
        elements_ += 10;
    }
    void fight() {
        Camp first = (flag == red || (flag == none && number % 2)) ? red : blue;
        if (RedWarrior == nullptr || BlueWarrior == nullptr) {
            two_are_both_alive_before = false;
            Won_Warrior = nullptr;
            return;
        } 
        two_are_both_alive_before = RedWarrior->isAlive() && BlueWarrior->isAlive();
        if (RedWarrior->Type() == lion)
            ((Lion*)(RedWarrior))->elements_before_fighting = RedWarrior->elements();
        if (BlueWarrior->Type() == lion)
            ((Lion*)(BlueWarrior))->elements_before_fighting = BlueWarrior->elements();
        if (first == red) {
            RedWarrior->attack(*BlueWarrior, true);
        } else {
            BlueWarrior->attack(*RedWarrior, true);
        }
        if (RedWarrior->isAlive() && BlueWarrior->isAlive()) {//draw
            if (RedWarrior->Type() == dragon) {
                ((Dragon*)RedWarrior)->morale_change(-0.2);
                if (RedWarrior->camp() == first)
                    ((Dragon*)RedWarrior)->yell();
            }
            if (BlueWarrior->Type() == dragon) {
                ((Dragon*)BlueWarrior)->morale_change(-0.2);
                if (BlueWarrior->camp() == first)
                    ((Dragon*)BlueWarrior)->yell();
            }
            if (RedWarrior->Type() == lion)
                ((Lion*)RedWarrior)->loyaltyLose();
            if(BlueWarrior->Type() == lion)
                ((Lion*)BlueWarrior)->loyaltyLose();
            last_result = none;
            Won_Warrior = nullptr;
        } else if (!RedWarrior->isAlive() && !BlueWarrior->isAlive()) {
            Won_Warrior = nullptr;
        } else {
            Warrior* winner;
            Warrior* loser;
            if (RedWarrior->isAlive() && !BlueWarrior->isAlive()) {
                winner = RedWarrior;
                loser = BlueWarrior;
                Won_Warrior = RedWarrior;
            } else if (!RedWarrior->isAlive() && BlueWarrior->isAlive()) {
                winner = BlueWarrior;
                loser = RedWarrior;
                Won_Warrior = BlueWarrior;
            }
            Camp this_Result = winner->camp();
            if (this_Result == last_result) {
                if (flag != this_Result) {
                    flag = this_Result;
                    flag_Raised = true;
                }
            }
            last_result = this_Result;
            if (two_are_both_alive_before)
                loser->print_death_info();
            if (winner->Type() == dragon) {
                ((Dragon*)(winner))->morale_change(0.2);
                if (first == winner->camp())
                    ((Dragon*)winner)->yell();
            } else if (winner->Type() == wolf) {
                ((Wolf*)winner)->grab_Weapon(*loser);
            }
            if (loser->Type() == lion) {
                winner->add_elements(((Lion*)loser)->elements_before_fighting > 0 ? ((Lion*)loser)->elements_before_fighting : 0);
            }
            if (loser->camp() == red) {
                RedWarrior = nullptr;
            } else BlueWarrior = nullptr;
            delete loser;
            winner->print_earning_info(elements_);
            if (flag_Raised) {
                flag_Raised = false;
                PrintTime();
                printf("%s flag raised in city %d\n", flag == red ? "red" : "blue", number);
            }
        } 
    }
    void check_lions_to_run() {
        if (RedWarrior != nullptr && RedWarrior->Type() == lion && ((Lion*)RedWarrior)->is_to_run_away()) {
            delete RedWarrior;
            RedWarrior = nullptr;
        }
        if (BlueWarrior != nullptr && BlueWarrior->Type() == lion && ((Lion*)BlueWarrior)->is_to_run_away()) {
            delete BlueWarrior;
            BlueWarrior = nullptr;
        }
    }
    void give_elements() {
        if (RedWarrior == nullptr && BlueWarrior != nullptr) {
            BlueWarrior->get_elements_for_HQ(elements_);
            elements_ = 0;
        }
        else if (BlueWarrior == nullptr && RedWarrior != nullptr) {
            RedWarrior->get_elements_for_HQ(elements_);
            elements_ = 0;
        }
    }
    void give_elements_and_print() {
        if (RedWarrior == nullptr && BlueWarrior != nullptr) {
            BlueWarrior->get_elements_for_HQ(elements_);
            BlueWarrior->print_earning_info(elements_);
            elements_ = 0;
        }
        else if (BlueWarrior == nullptr && RedWarrior != nullptr) {
            RedWarrior->get_elements_for_HQ(elements_);
            RedWarrior->print_earning_info(elements_);
            elements_ = 0;
        }
    }
    virtual void reaching_Info() {
        if (RedWarrior != nullptr)
            RedWarrior->output_reaching_information();
        if (BlueWarrior != nullptr)
            BlueWarrior->output_reaching_information();
    }
    void check_battlefield() {
        if (RedWarrior != nullptr && RedWarrior->elements() <= 0) {
            delete RedWarrior;
            RedWarrior = nullptr;
        }
        if (BlueWarrior != nullptr && BlueWarrior->elements() <= 0) {
            delete BlueWarrior;
            BlueWarrior = nullptr;
        }
    }
};

class HeadQ : public City{
private:
    int Warrior_Number = 1;
    bool is_taken_ = false;
    Warrior* get_a_pointer_of_a_Warrior(WarriorType Type) {
        switch (Type) {
            case ninja: return new Ninja(Warrior_Number, InitElements[Type], InitForces[Type], *this);
            case iceman: return new Iceman(Warrior_Number, InitElements[Type], InitForces[Type], *this);
            case dragon: return new Dragon(Warrior_Number, InitElements[Type], InitForces[Type], *this);
            case lion: return new Lion(Warrior_Number, InitElements[Type], InitForces[Type], *this);
            case wolf: return new Wolf(Warrior_Number, InitElements[Type], InitForces[Type], *this);
            default: return nullptr;
        }
        return nullptr;
    }
public:
    Camp camp;
    Warrior* enemy = nullptr;
    void be_taken() {
        is_taken_ = true;
    }
    bool is_taken() {
        return is_taken_;
    }
    static const WarriorType RedSeq[5];
    static const WarriorType BlueSeq[5];
    HeadQ(int number, int elements, Camp camp) : City(number, elements), camp(camp) {}
    void spawnWarrior() {
        WarriorType spawnType = ((camp == red) ? RedSeq[(Warrior_Number-1)%5] : BlueSeq[(Warrior_Number-1)%5]);
        if (elements_ >= InitElements[spawnType]) {
            elements_ -= InitElements[spawnType];
            if (camp == red) {
                RedWarrior = get_a_pointer_of_a_Warrior(spawnType);
            } else {
                BlueWarrior = get_a_pointer_of_a_Warrior(spawnType);
            }
            ++Warrior_Number;
        }
        
    }
    void getElements(int gain) {
        elements_ += gain;
    }
    void reportElements() {
        PrintTime();
        printf("%d elements in %s headquarter\n", elements_, camp == red ? "red" : "blue");
    }
    void reaching_Info() {
        if (camp == red) {
            if (BlueWarrior != nullptr && BlueWarrior != enemy) {
                BlueWarrior->output_reaching_information();
                enemy = BlueWarrior;
                if (is_taken_) {
                    PrintTime();
                    printf("red headquarter was taken\n");
                }
            }
        } else if (RedWarrior != nullptr && RedWarrior != enemy) {
            RedWarrior->output_reaching_information();
            enemy = RedWarrior;
            if (is_taken_) {
                PrintTime();
                printf("blue headquarter was taken\n");
            }
        }
    }
};

const WarriorType HeadQ::RedSeq[5] = {iceman, lion, wolf, ninja, dragon};
const WarriorType HeadQ::BlueSeq[5] = {lion, dragon, ninja, iceman, wolf};

int main() {
    int t;
    cin >> t;
    for (int Case = 1; Case <= t; ++Case) {
        printf("Case %d:\n", Case);
        cin >> M >> N >> R >> K >> T;
        for (int i = 0; i < 5; ++i)
            cin >> InitElements[i];
        for (int i = 0; i < 5; ++i)
            cin >> InitForces[i];
        
        City* AllCities[N+2];

        HeadQ RedHQ = HeadQ(0, M, red);
        HeadQ BlueHQ = HeadQ(N+1, M, blue);
        AllCities[0] = &RedHQ;
        for (int i = 1; i <= N; ++i)
            AllCities[i] = new City(i, 0);
        AllCities[N+1] = &BlueHQ;

        Time = 0;

        while (Time <= T) {
            //:00 spawn
            RedHQ.spawnWarrior();
            BlueHQ.spawnWarrior();
            Time += 5;
            if (Time > T) break;
            //:05 lions run away
            for (int i = 0; i <= N+1; ++i) {
                AllCities[i]->check_lions_to_run();
            }
            Time += 5;
            if (Time > T) break;
            //:10 warriors move
            if (AllCities[1]->BlueWarrior != nullptr) {
                AllCities[1]->BlueWarrior->move(-1);
                if (RedHQ.BlueWarrior != nullptr) {
                    delete RedHQ.BlueWarrior;
                    RedHQ.BlueWarrior = AllCities[1]->BlueWarrior;
                    AllCities[1]->BlueWarrior = nullptr;
                    RedHQ.be_taken();
                } else {
                    RedHQ.BlueWarrior = AllCities[1]->BlueWarrior;
                    AllCities[1]->BlueWarrior = nullptr;
                } 
            }
            for (int i = 2; i <= N+1; ++i) {
                if (AllCities[i]->BlueWarrior != nullptr) {
                    AllCities[i]->BlueWarrior->move(-1);
                    AllCities[i-1]->BlueWarrior = AllCities[i]->BlueWarrior;
                    AllCities[i]->BlueWarrior = nullptr;
                }
            }
            if (AllCities[N]->RedWarrior != nullptr) {
                AllCities[N]->RedWarrior->move(1);
                if (BlueHQ.RedWarrior != nullptr) {
                    delete BlueHQ.RedWarrior;
                    BlueHQ.RedWarrior = AllCities[N]->RedWarrior;
                    AllCities[N]->RedWarrior = nullptr;
                    BlueHQ.be_taken();
                } else {
                    BlueHQ.RedWarrior = AllCities[N]->RedWarrior;
                    AllCities[N]->RedWarrior = nullptr;
                }

            }
            for (int i = N-1; i >= 0; --i) {
                if (AllCities[i]->RedWarrior != nullptr) {
                    AllCities[i]->RedWarrior->move(1);
                    AllCities[i+1]->RedWarrior = AllCities[i]->RedWarrior;
                    AllCities[i]->RedWarrior = nullptr;
                }
            }
            for (int i = 0; i <= N+1; ++i)
                AllCities[i]->reaching_Info();
            if (RedHQ.is_taken() || BlueHQ.is_taken()) break;
            Time += 10;
            if (Time > T) break;
            //:20 cities producing elements
            for (int i = 1; i < N+1; ++i) {
                AllCities[i]->produce();
            }
            Time += 10;
            if (Time > T) break;
            //:30 Warriors earning elements
            for (int i = 1; i < N+1; ++i) {
                AllCities[i]->give_elements_and_print();
            }
            Time += 5;
            if (Time > T) break;
            //:35 warriors shoot
            for (int i = 1; i < N+1; ++i) {
                if (AllCities[i]->RedWarrior != nullptr)
                    AllCities[i]->RedWarrior->shoot(AllCities[i+1]->BlueWarrior);
                if (AllCities[i]->BlueWarrior != nullptr)
                    AllCities[i]->BlueWarrior->shoot(AllCities[i-1]->RedWarrior);
            }
            Time += 3;
            if (Time > T) break;
            //:38 Warriors use bomb
            for (int i = 1; i < N+1; ++i) {
                if ((AllCities[i]->RedWarrior != nullptr &&
                AllCities[i]->RedWarrior->useBomb(AllCities[i]->BlueWarrior, AllCities[i]->flag, AllCities[i]->number)) || 
                (AllCities[i]->BlueWarrior != nullptr &&
                AllCities[i]->BlueWarrior->useBomb(AllCities[i]->RedWarrior, AllCities[i]->flag, AllCities[i]->number))) {
                    delete AllCities[i]->RedWarrior;
                    AllCities[i]->RedWarrior = nullptr;
                    delete AllCities[i]->BlueWarrior;
                    AllCities[i]->BlueWarrior = nullptr;
                }
            }
            Time += 2;
            if (Time > T) break;
            //:40 Warriors attack
            for (int i = 1; i < N+1; ++i) {
                AllCities[i]->fight();
                AllCities[i]->check_battlefield();
            }
            for (int i = 1; i < N+1; ++i) {
                if (AllCities[i]->Won_Warrior != nullptr && AllCities[i]->Won_Warrior->camp() == blue) {
                    AllCities[i]->Won_Warrior->get_rewards_from_HQ();
                }
            }
            for (int i = N; i > 0; --i) {
                if (AllCities[i]->Won_Warrior != nullptr && AllCities[i]->Won_Warrior->camp() == red) {
                    AllCities[i]->Won_Warrior->get_rewards_from_HQ();
                }
            }
            for (int i = 1; i < N+1; ++i) {
                if (AllCities[i]->Won_Warrior != nullptr) {
                    AllCities[i]->give_elements();
                }
            }
            Time += 10;
            if (Time > T) break;
            //:50 HQs report current elements
            RedHQ.reportElements();
            BlueHQ.reportElements();
            Time += 5;
            if (Time > T) break;
            //:55 Warriors report weapons
            for (int i = 0; i <= N+1; ++i)
                if (AllCities[i]->RedWarrior != nullptr)
                    AllCities[i]->RedWarrior->reportWeapons();
            for (int i = 0; i <= N+1; ++i)
                if (AllCities[i]->BlueWarrior != nullptr)
                    AllCities[i]->BlueWarrior->reportWeapons();
            Time += 5;
            //:00
        }
    }
    return 0;
}

Dragon::Dragon(int number, int elements, int force, HeadQ& HQ) : Warrior(dragon, number, elements, force, HQ) {
    morale = double(HQ.elements()) / InitElements[dragon];
    printf("Its morale is ");
    cout << std::fixed << std::setprecision(2) << morale << std::endl;
}

Lion::Lion(int number, int elements, int force, HeadQ &HQ) : Warrior(lion, number, elements, force, HQ) {
    loyalty = HQ.elements();
    printf("Its loyalty is %d\n", loyalty);
}

void Warrior::get_elements_for_HQ(int gain) {
    HQ.getElements(gain);
}
void Warrior::get_rewards_from_HQ() {
    if (HQ.elements() >= 8) {
        HQ.getElements(-8);
        elements_ += 8;
    }
}
Warrior::Warrior(WarriorType Type, int number, int elements, int force, HeadQ& HQ) : Type_(Type), number_(number), elements_(elements), force(force), HQ(HQ), camp_(HQ.camp), location(HQ.number) {
    getWeapon();
    PrintTime();
    printf("%s %s %d born\n", this->getCamp(), this->getName(), this->number_);
}