#include <iostream>
#include <string>
#include <vector>

class Warrior;
class HeadQuarter;

enum WarriorType {dragon, ninja, iceman, lion, wolf};
enum WeaponType {sword, bomb, arrow};
enum Camp {red, blue};

const std::string Weapons[3] = {"sword", "bomb", "arrow"};
const std::string Camps[2] = {"red", "blue"};

int WarriorElements[5] = {0};

class HeadQuarter {
private:
    std::vector<Warrior*> Wqueue;
    Camp camp;
    int HP;
    WarriorType sequence[5];
    int numbers[5] = {0};
    int current = 0;
    int number = 1;
public:
    HeadQuarter(WarriorType sequence[5], Camp camp, int HP) : camp(camp), HP(HP) {
        for (int i = 0; i < 5; i++) {
            this->sequence[i] = sequence[i];
        }
    }
    int Wnum(WarriorType wt) {return numbers[wt];}
    Warrior* create(WarriorType wt);
    bool spawn(int Time);
    void end(int Time);
    ~HeadQuarter() {}
};

class Warrior{
private:
    WarriorType wt_;
    Camp camp_;
    int element_;
    const int number_;
public:
    Warrior(WarriorType type, int HP, Camp camp, int number) : wt_(type), element_(HP), camp_(camp), number_(number) {}
    const Camp camp() {return camp_;}
    const int number() {return number_;}
    const int HP() {return element_;}
    const WarriorType wt() {return wt_;}
    virtual void output(int time, HeadQuarter& HQ) = 0;
    virtual ~Warrior() {}
};

class Weapon {
private:
    WeaponType wt_;
    int attack;
public:
    Weapon(WeaponType wt, int attack) : wt_(wt), attack(attack) {}
    WeaponType wt() {return wt_;}
    virtual ~Weapon() {}
};

class Dragon : public Warrior {
private:
    Weapon w;
    float morale;
public:
    Dragon(int HP, Camp camp, int number, WeaponType wt, int Wattack, float morale) : Warrior(dragon, HP, camp, number), w(wt, Wattack), morale(morale) {}
    virtual void output(int time, HeadQuarter& HQ) {
        printf("%03d %s dragon %d born with strength %d,%d dragon in %s headquarter\n", time, Camps[camp()].c_str(), number(), HP(), HQ.Wnum(wt()), Camps[camp()].c_str());
        printf("It has a %s,and it's morale is %.2f\n", Weapons[w.wt()].c_str(), morale);
    }
    virtual ~Dragon() {}
};

class Ninja : public Warrior {
private:
    Weapon w1;
    Weapon w2;
public:
    Ninja(int HP, Camp camp, int number, WeaponType wt, int Wattack, WeaponType wt2, int Wattack2) : Warrior(ninja, HP, camp, number), w1(wt, Wattack), w2(wt2, Wattack2) {}
    virtual void output(int time, HeadQuarter& HQ) {
        printf("%03d %s ninja %d born with strength %d,%d ninja in %s headquarter\n", time, Camps[camp()].c_str(), number(), HP(), HQ.Wnum(wt()), Camps[camp()].c_str());
        printf("It has a %s and a %s\n", Weapons[w1.wt()].c_str(), Weapons[w2.wt()].c_str());
    }
    virtual ~Ninja() {}
};

class Iceman : public Warrior {
private:
    Weapon w;
public:
    Iceman(int HP, Camp camp, int number, WeaponType wt, int Wattack) : Warrior(iceman, HP, camp, number), w(wt, Wattack) {}
    virtual void output(int time, HeadQuarter& HQ) {
        printf("%03d %s iceman %d born with strength %d,%d iceman in %s headquarter\n", time, Camps[camp()].c_str(), number(), HP(), HQ.Wnum(wt()), Camps[camp()].c_str());
        printf("It has a %s\n", Weapons[w.wt()].c_str());
    }
    virtual ~Iceman() {}
};

class Lion : public Warrior {
private:
    int loyalty;
public:
    Lion(int HP, Camp camp, int number, int loyalty) : Warrior(lion, HP, camp, number), loyalty(loyalty) {}
    virtual void output(int time, HeadQuarter& HQ) {
        printf("%03d %s lion %d born with strength %d,%d lion in %s headquarter\n", time, Camps[camp()].c_str(), number(), HP(), HQ.Wnum(wt()), Camps[camp()].c_str());
        printf("It's loyalty is %d\n", loyalty);
    }
    virtual ~Lion() {}
};

class Wolf :public Warrior {
public:
    Wolf(int HP, Camp camp, int number) : Warrior(wolf, HP, camp, number) {}
    virtual void output(int time, HeadQuarter& HQ) {
        printf("%03d %s wolf %d born with strength %d,%d wolf in %s headquarter\n", time, Camps[camp()].c_str(), number(), HP(), HQ.Wnum(wt()), Camps[camp()].c_str());
    }
    virtual ~Wolf() {}
};

int main() {
    int cases = 0;
    std::cin >> cases;

    for (int Case = 1; Case <= cases; Case++) {
        printf("Case:%d\n", Case);
        int M = 0;
        std::cin >> M;
        for (int i = 0; i < 5; i++) {
            std::cin >> WarriorElements[i];
        }

        WarriorType redSequence[5] = {iceman, lion, wolf, ninja, dragon};
        HeadQuarter redHQ(redSequence, red, M);
        WarriorType blueSequence[5] = {lion, dragon, ninja, iceman, wolf};
        HeadQuarter blueHQ(blueSequence, blue, M);

        std::vector<HeadQuarter*> HQseq;
        HQseq.push_back(&redHQ), HQseq.push_back(&blueHQ);
        for(int Time = 0; ; Time++) {
            for (std::vector<HeadQuarter*>::iterator i = HQseq.begin(); i < HQseq.end(); i++) {
                if (!(*i)->spawn(Time)) {
                    (*i)->end(Time);
                    HQseq.erase(i--);
                }
            }
            if (HQseq.empty()) break;
        }
    }
    return 0;
}

Warrior* HeadQuarter::create(WarriorType wt) {
    Warrior* w = nullptr;
    switch (wt) {
        case dragon: w = new Dragon(WarriorElements[dragon], camp, number, WeaponType(number%3), 0, (float(HP)) / WarriorElements[dragon]); break;
        case ninja: w = new Ninja(WarriorElements[ninja], camp, number, WeaponType(number%3), 0, WeaponType((number+1)%3), 0); break;
        case iceman: w = new Iceman(WarriorElements[iceman], camp, number, WeaponType(number%3), 0); break;
        case lion: w = new Lion(WarriorElements[lion], camp, number, HP); break;
        case wolf: w = new Wolf(WarriorElements[wolf], camp, number); break;
        default: break;
    }
    number++;
    return w;
}

bool HeadQuarter::spawn(int Time) {
    for(int i = current; i < 5 + current; i++) {
        WarriorType wt = sequence[i % 5];
        int neededHP = WarriorElements[wt];
        if (HP >= neededHP) {
            HP -= neededHP;
            numbers[wt]++;
            Warrior* w = create(wt);
            Wqueue.push_back(w);
            w->output(Time, *this);
            current = (i + 1) % 5;
            return true;
        }
    }
    return false;
}

void HeadQuarter::end(int Time) {
    printf("%03d %s headquarter stops making warriors\n", Time, Camps[camp].c_str());
}