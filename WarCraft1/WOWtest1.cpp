#include <iostream>
#include <string>
#include <vector>

enum WarriorType {
    Dragon, Ninja, Iceman, Lion, Wolf
};

enum Camp {
    red, blue
};


class WarriorBase {
private:
    const char* name;
    WarriorType type;
    int HP;
public:
    WarriorBase(const char* name, WarriorType type, int HP) : name(name), type(type), HP(HP) {}
    int getHP() {
        return HP;
    }
    const char* getName() {
        return name;
    }
    WarriorType getType() {
        return type;
    }
};

class Warrior : public WarriorBase {
private:
    int attack;
    Camp camp;
    const int number;
public:
    Warrior(WarriorBase &base, int attack, Camp camp, int number) : WarriorBase(base), attack(attack), camp(camp), number(number) {}
    int getNumber() {
        return number;
    }
};

WarriorBase *WarriorBases = NULL;

class HeadQuarter {
private:
    std::vector<Warrior*> Wqueue;
    Camp camp;
    const char* campName;
    int HP;
    WarriorType sequence[5];
    int numbers[5] = {0};
    int current = 0;
    int number = 1;
    void output(Warrior* temp, int Time) {
        printf("%03d %s %s %d born with strength %d,%d %s in %s headquarter\n", Time, campName, temp->getName(), temp->getNumber(), temp->getHP(), numbers[temp->getType()], temp->getName(), campName);
    }
public:
    HeadQuarter(WarriorType sequence[5], const char* campName, Camp camp, int HP) : campName(campName), camp(camp), HP(HP) {
        for (int i = 0; i < 5; i++) {
            this->sequence[i] = sequence[i];
        }
    }
    bool spawn(WarriorBase *base, int Time) {
        for(int i = current; i < 5; i++) {
            if (HP >= base[sequence[i]].getHP()) {
                HP -= base[sequence[i]].getHP();
                Warrior *temp = new Warrior(base[sequence[i]], 0, camp, number++);
                numbers[temp->getType()]++;
                output(temp, Time);
                Wqueue.push_back(temp);
                current = (i + 1) % 5;
                return true;
            }
        }
        for(int i = 0; i < current; i++) {
            if (HP >= base[sequence[i]].getHP()) {
                HP -= base[sequence[i]].getHP();
                Warrior *temp = new Warrior(base[sequence[i]], 0, camp, number++);
                numbers[temp->getType()]++;
                output(temp, Time);
                Wqueue.push_back(temp);
                current = (i + 1) % 5;
                return true;
            }
        }
        return false;
    }
    void end(int Time) {
        printf("%03d %s headquarter stops making warriors\n", Time, campName);
    }
};

int main() {
    int cases = 0;
    std::cin >> cases;

    for (int Case = 1; Case <= cases; Case++) {
        printf("Case:%d\n", Case);
        int HPs[5] = {0};
        int M = 0;
        std::cin >> M;
        for (int i = 0; i < 5; i++) {
            std::cin >> HPs[i];
        }
        WarriorType redSequence[5] = {Iceman,Lion,Wolf,Ninja,Dragon};
        HeadQuarter redHQ(redSequence, "red", red, M);
        WarriorType blueSequence[5] = {Lion, Dragon, Ninja,Iceman, Wolf};
        HeadQuarter blueHQ(blueSequence, "blue", blue, M);
        WarriorBase tplWarBase[5] = {
            WarriorBase("dragon", Dragon, HPs[0]),
            WarriorBase("ninja", Ninja, HPs[1]),
            WarriorBase("iceman", Iceman, HPs[2]),
            WarriorBase("lion", Lion, HPs[3]),
            WarriorBase("wolf", Wolf, HPs[4])
        };
        std::vector<HeadQuarter*> HQseq;
        HQseq.push_back(&redHQ), HQseq.push_back(&blueHQ);
        for(int Time = 0; ; Time++) {
            for (std::vector<HeadQuarter*>::iterator i = HQseq.begin(); i < HQseq.end(); i++) {
                if (!(*i)->spawn(tplWarBase, Time)) {
                    (*i)->end(Time);
                    HQseq.erase(i--);
                }
            }
            if (HQseq.empty()) break;
        }
    }
    return 0;
}