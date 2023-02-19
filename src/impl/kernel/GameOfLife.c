#include "funcs.h"

#define WidthOBoard 80
#define HeightOBoard 25

uint8_t dead = PRINT_COLOR_BLACK | PRINT_COLOR_BLACK << 4;
uint8_t alive = PRINT_COLOR_YELLOW | PRINT_COLOR_YELLOW << 4;

uint8_t GOLboard[HeightOBoard][WidthOBoard];

void drawScreen();
void update();
void GOLinit();

void GOL_Mainloop() {
    GOLinit();
    MSDelay(300000);
    while (1) {
        drawScreen();
        update();
        int seen = 0;
        for (int i=0; i<HeightOBoard; i++) {
            for (int j=0; j<WidthOBoard; j++) {
                if (GOLboard[i][j] == 1) {
                    seen ++;
                }
            }
        }
        if (seen == 0) {
            break;
        }
        // TODO: if nothing moving break.
        MSDelay(300000);
    }
}

void GOLinit() {
    int random = 0;
    for (int i=0; i<HeightOBoard; i++) {
        for (int j=0; j<WidthOBoard; j++) {
            random = rand()%100;
            if (random >= 75) {
                GOLboard[i][j] = 1;
            } else {
                GOLboard[i][j] = 0;
            }
        }
    }
}

void drawScreen() {
    for (int i=0; i<HeightOBoard; i++) {
        for (int j=0; j<WidthOBoard; j++) {
            printPos(j, i, (GOLboard[i][j] == 0)? dead : alive, ' ');
        }
    }
}

void update() {
    int suround, prevx, prevy, postx, posty;
    uint8_t nGOLboard[HeightOBoard*WidthOBoard][3];
    int count = 0;
    //Every piece with iether 2 or 3 neighbors survives
    for (int i=0; i<HeightOBoard; i++) {
        for (int j=0; j<WidthOBoard; j++) {
            prevx = (i != 0)? i-1: 0;
            postx = (i != HeightOBoard-1)? i+1: HeightOBoard-1;
            prevy = (j != 0)? j-1: 0;
            posty = (j != WidthOBoard-1)? j+1: WidthOBoard-1;
            suround = 0;
            for (int k=prevx; k<postx+1; k++){
                for (int l=prevy; l<posty+1; l++){
                    suround += (GOLboard[k][l] == 1 && !(k == i && l == j))? 1:0;
                }
            }
            if ((((GOLboard[i][j] == 1 && suround == 2) || (suround == 3)) && GOLboard[i][j] == 0) || (!((GOLboard[i][j] == 1 && suround == 2) || (suround == 3)) && GOLboard[i][j] == 1)){
                nGOLboard[count][2] = (GOLboard[i][j] == 0)? 1: 0;
                nGOLboard[count][1] = j;
                nGOLboard[count][0] = i;
                count ++;
            }
        }
    }
    for (int i=0; i<count; i++){
        GOLboard[nGOLboard[i][0]][nGOLboard[i][1]] = nGOLboard[i][2];
    }
}