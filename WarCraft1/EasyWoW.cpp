#include <iostream>

int main() {
    int Cases;
    std::cin >> Cases;
    char* namestr[5] = {"dragon", "ninja", "iceman", "lion", "wolf"};
    for(int Case = 1; Case <= Cases; Case++) {
        printf("Case:%d\n", Case);
        int M;
        std::cin >> M;
        int priStr[5];//D N I L W
        std::cin >> priStr[0] >> priStr[1] >> priStr[2] >> priStr[3] >> priStr[4];
        bool redExists = true;
        bool blueExists = true;
        int redSeq[5] = {2, 3, 4, 1, 0};//I L W N D
        int blueSeq[5] = {3, 0, 1, 2, 4};//L D N I W
        int redNumbers[5] = {0};
        int blueNumbers[5] = {0};
        int redLoc = 0;
        int blueLoc = 0;
        int redNum = 1;
        int blueNum = 1;
        int redM = M;
        int blueM = M;
        for(int time = 0; ; time++) {
            if(!(redExists || blueExists)) {
                break;
            }
            if(redExists) {
                bool doOutput = false;
                for(int i = redLoc; i < 5; i++) {
                    if(priStr[redSeq[i]] <= redM) {
                        redM -= priStr[redSeq[i]];
                        redNumbers[redSeq[i]]++;
                        printf("%03d red %s %d born with strength %d,%d %s in red headquarter\n",
                        time, namestr[redSeq[i]], redNum, priStr[redSeq[i]], redNumbers[redSeq[i]], namestr[redSeq[i]]);
                        doOutput = true;
                        redLoc = (i + 1) % 5;
                        redNum++;
                        break;
                    }
                }
                if(!doOutput) {
                    for(int i = 0; i < redLoc; i++) {
                        if(priStr[redSeq[i]] <= redM) {
                            redM -= priStr[redSeq[i]];
                            redNumbers[redSeq[i]]++;
                            printf("%03d red %s %d born with strength %d,%d %s in red headquarter\n",
                            time, namestr[redSeq[i]], redNum, priStr[redSeq[i]], redNumbers[redSeq[i]], namestr[redSeq[i]]);
                            doOutput = true;
                            redLoc = (i + 1) % 5;
                            redNum++;
                            break;
                        }
                    }
                }
                if(!doOutput) {
                    printf("%03d red headquarter stops making warriors\n", time);
                    redExists = false;
                }
            }
            if(blueExists) {
                bool doOutput = false;
                for(int i = blueLoc; i < 5; i++) {
                    if(priStr[blueSeq[i]] <= blueM) {
                        blueM -= priStr[blueSeq[i]];
                        blueNumbers[blueSeq[i]]++;
                        printf("%03d blue %s %d born with strength %d,%d %s in blue headquarter\n",
                        time, namestr[blueSeq[i]], blueNum, priStr[blueSeq[i]], blueNumbers[blueSeq[i]], namestr[blueSeq[i]]);
                        doOutput = true;
                        blueLoc = (i + 1) % 5;
                        blueNum++;
                        break;
                    }
                }
                if(!doOutput) {
                    for(int i = 0; i < blueLoc; i++) {
                        if(priStr[blueSeq[i]] <= blueM) {
                            blueM -= priStr[blueSeq[i]];
                            blueNumbers[blueSeq[i]]++;
                            printf("%03d blue %s %d born with strength %d,%d %s in blue headquarter\n",
                            time, namestr[blueSeq[i]], blueNum, priStr[blueSeq[i]], blueNumbers[blueSeq[i]], namestr[blueSeq[i]]);
                            doOutput = true;
                            blueLoc = (i + 1) % 5;
                            blueNum++;
                            break;
                        }
                    }
                }
                if(!doOutput) {
                    printf("%03d blue headquarter stops making warriors\n", time);
                    blueExists = false;
                }
            }
        }
    }
    return 0;
}