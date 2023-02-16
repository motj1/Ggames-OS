#include "funcs.h"

uint8_t colour = PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4;
uint8_t wall = PRINT_COLOR_LIGHT_GRAY | PRINT_COLOR_LIGHT_GRAY << 4;
uint8_t head = PRINT_COLOR_DARK_GRAY | PRINT_COLOR_DARK_GRAY << 4;
uint8_t body = PRINT_COLOR_GREEN | PRINT_COLOR_GREEN << 4;
uint8_t fruit = PRINT_COLOR_LIGHT_GREEN | PRINT_COLOR_LIGHT_GREEN << 4;

int snakeX[200] = { 0 }, snakeY[200] = { 0 }, score = 0;
int oX = 0, oY = 0, ofX = 0, ofY = 0;
unsigned int fruitX, fruitY;

void refresh();
char getIn(char inS);
int logic();
void init();

void SnakegameLoop() {
    init();
    char last = KEY_LEFT;
    while (1) {
        refresh();
        last = getIn(last);
        if (logic() == -1) { refresh(); break; }
        MSDelay(30000); // 100000
    }
    for (int i=score+1; i > -1; i--) {
        printPos(snakeX[i], snakeY[i], colour, ' ');
        // buffer[snakeX[i] + NUM_COLS * snakeY[i]] = (struct Char) { character: (uint8_t) ' ', color: color, };
        MSDelay(50000); // 100000
    }
}

void init() {
    score = 0;
    snakeX[0] = 40;
    snakeY[0] = 13;
    fruitX = rand()%80; 
    fruitY = rand()%23;
    oX = snakeX[score];
    oY = snakeY[score];
    ofX = fruitX;
    ofY = fruitY;

    int printed;
    print_clear();
    for (int i=0; i<23; i++) {
        print_char('\n');
        for (int j=0; j<80; j++) {
            if (i == 0 || i == 22 || j == 0 || j == 79) {
                print_char_color('#', wall);
            } else {
                print_char(' ');
            }
        }
    }
    print_char('\n');
}

void refresh() {
    printPos(oX, oY, colour, ' ');
    printPos(ofX, ofY, colour, ' ');

    printPos(snakeX[0], snakeY[0], head, 'O');

    printPos(fruitX, fruitY, fruit, '*');

    for (int i=0; i < score; i++) {
        printPos(snakeX[i+1], snakeY[i+1], body, 'o');
    }
    int num = score;

    if (num == 0) { printPos(1, 0, colour, '0'); /*buffer[1] = (struct Char) { character: (uint8_t) '0', color: color, }; */ return; }
    char toprint[20];
    int i = 0;
    while (num != 0) {
        toprint[i] = (num%10) + 48;
        num -= num%10;
        num /= 10;
        i ++;
    }
    for (int j = i-1, k = 0; j > -1; j--) {
        k ++;
        printPos(k, 0, colour, toprint[j]);
    }
}

char getIn(char inS) {
    oX = snakeX[score];
    oY = snakeY[score];

    for (int i=score+1; i > 0; i--) {
        snakeX[i] = snakeX[i-1];
        snakeY[i] = snakeY[i-1];
    }

    char in = getC();
    int storX = snakeX[0], storY = snakeY[0];
    if (in == KEY_LEFT)
        snakeX[0] --;
    else if (in == KEY_RIGHT)
        snakeX[0] ++;
    else if (in == KEY_UP)
        snakeY[0] --;
    else if (in == KEY_DOWN)
        snakeY[0] ++;
    else {
        if (in != inS) {
            in = -1;
        }
    }

    return in;
}

int logic() {
    if (fruitX == snakeX[0] && fruitY == snakeY[0]) {
        score ++;
        ofX = fruitX;
        ofY = fruitY;
        fruitX = rand()%70 + 5; 
        fruitY = rand()%21 + 1;
    }
    // Wall collision
    if (snakeX[0] >= 78) {
        // snakeX[0] = 2;
        return -1;
    } 
    if (snakeY[0] >= 22) {
        // snakeY[0] = 2;
        return -1;
    }
    if (snakeX[0] <= 1) {
        // snakeX[0] = 77;
        return -1;
    } if (snakeY[0] <= 1) {
        // snakeY[0] = 21;
        return -1;
    }
    // Hitting self
    // for (int i=score+1; i > 0; i--) {
    //     if (snakeX[i] == snakeX[0] && snakeY[i] == snakeY[0]) {
    //         return -1;
    //     }
    // }
    return 0;
}