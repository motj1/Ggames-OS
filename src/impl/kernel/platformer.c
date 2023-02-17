#include "funcs.h"

int w = 25, h = 80;

typedef struct playerStruct {
    int x, y, vertMove, horzMove, timeup, ox, oy;
} playerStruct;

typedef struct enemyStruct {
    int x, y;
} enemyStruct;

typedef struct obsticle {
    int x, y;
} obsticle;

playerStruct player = { 0, 0, 0, 0, 0, 0, 0 };
enemyStruct enemy = { 10, 0 };

obsticle walls[10];

uint8_t nada = PRINT_COLOR_BLACK | PRINT_COLOR_BLACK << 4;
uint8_t frame = PRINT_COLOR_LIGHT_GRAY | PRINT_COLOR_LIGHT_GRAY << 4;
uint8_t pColour = PRINT_COLOR_PINK | PRINT_COLOR_PINK << 4;
uint8_t blood = PRINT_COLOR_RED | PRINT_COLOR_RED << 4;

void initalise();
void Draw();
void getInput();
int Logic(int tick);

void PlatformerGameLoop() {
    int tick = 0;
    initalise();
    MSDelay(300000);
    while (1) {
        tick ++;
        Draw();

        getInput();

        if (Logic(tick) == -1) break;

        tick %= 59;
        MSDelay(30000);
    }
    for (int i=-1; i < 2; i++) {
        for (int j=-1; j < 2; j++) {
            if (i != 0 || j != 0)
                printPos(player.x + 1 + i, 23 - player.y + j, blood, '#');
            MSDelay(50000);
        }
    }
}

void initalise() {
    for (int i=0; i<10; i++) {
        walls[i].x = i+10;
        walls[i].y = 0;
    }
    print_clear();
    for (int i=0; i<25; i++) {
        print_char('\n');
        for (int j=0; j<80; j++) {
            if (i == 0 || i == 24 || j == 0 || j == 79) {
                print_char_color('#', frame);
            } else {
                print_char(' ');
            }
        }
    }
}

void Draw() {
    for (int i=0; i<23; i++) {
        for (int j=0; j<78; j++) {
            int seen = 0;
            for (int k=0; k<10; k++) {
                if (walls[k].x == j && walls[k].y == i)
                    printPos(j + 1, 23 - i, nada, ' ');
            }
            if (seen == 0)
                printPos(j + 1, 23 - i, nada, ' ');
        }
    }
    printPos(player.x + 1, 23 - player.y, pColour, 'O');
}

void getInput() {
    player.ox = player.x;
    player.oy = player.y;
    char ch = getC();
    if (ch == KEY_LEFT) {
        player.horzMove -= 2;
    } else if (ch == KEY_RIGHT) {
        player.horzMove += 2;
    } else if (ch == KEY_UP || ch == ' ') {
        player.vertMove += 2;
    } 
}

// int slogic(int playx, int playy) {
//     if (player.x + 1 <= playx + player.horzMove)
//         player.x ++;
//     else if (player.x - 1 >= playx + player.horzMove)
//         player.x --;
//     if (player.y + 1 <= playy + player.vertMove)
//         player.y ++;
//     else if (player.y - 1 >= playy + player.vertMove)
//         player.y --;
    
//     if (player.x == enemy.x && player.y == enemy.y)
//         return 0; // -1
//     if (player.y <= 0) {
//         player.y = 0;
//         player.vertMove = 0;
//     }

//     for (int i=0; i<10; i++) {
//         if (walls[i].x == player.x && walls[i].y == player.y) {
//             return -1;
//         }
//     }
//     return 0;
// }

int Logic(int tick) {
    // int playx = player.x;
    // int playy = player.y;
    // for (int i = 0; i < (player.horzMove > player.vertMove)? player.horzMove : player.vertMove; i++) {
    //     int tmpx = player.x, tmpy = player.y;
    //     if (slogic(playx, playy) == -1) {
    //         player.x = tmpx;
    //         player.y = tmpy;
    //         break;
    //     }
    // }
    player.x += player.horzMove;
    player.y += player.vertMove;

    if (tick % 15 == 0)
        player.horzMove -= (player.horzMove > 0)? 1 : (player.horzMove < 0)? -1 : 0;

    if (player.horzMove >= 5) {
        player.horzMove = 5;
    } else if (player.vertMove >= 2) {
        player.vertMove = 2;
    }

    if (player.x == enemy.x && player.y == enemy.y)
        return 0; // -1
    if (player.y <= 0) {
        player.y = 0;
        player.vertMove = 0;
    } else if (player.y >= 22) {
        player.y = 22;
        player.vertMove = (player.vertMove > 0)? player.vertMove * -1 : player.vertMove;
    } else {
        player.vertMove -= 2;
        if (player.y >= 4) {
            player.vertMove -= 2;
        }
    }
    if (player.x <= 0) {
        player.x = 0;
        player.horzMove = (player.horzMove < 0)? player.horzMove * -1 : player.horzMove;
    }
    if (player.x >= 77) {
        player.x = 77;
        player.horzMove = (player.horzMove > 0)? player.horzMove * -1 : player.horzMove;
    }
    return 0;
}