#include "funcs.h"

#define abs(num) (num < 0)? num * -1 : num;

int w = 25, h = 80;

typedef struct playerStruct {
    int x, y, vertMove, horzMove, timeup, ox, oy;
} playerStruct;

typedef struct enemyStruct {
    int x, y;
} enemyStruct;

typedef struct tile {
    int x, y, length;
} tile;

playerStruct player = { 0, 0, 0, 0, 0, 0, 0 };
enemyStruct enemy = { 10, 0 };

tile walls = { 11, 0, 10 };

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
            // if ((walls.x <= j && walls.x + walls.length >= j) && walls.y == i) {
            //     printPos(j + 1, 23 - i, frame, '#');
            // } else
            printPos(j + 1, 23 - i, nada, ' ');
        }
    }
    printPos(player.x + 1, 23 - player.y, pColour, 'O');
    printPos(enemy.x + 1, 23 - enemy.y, blood, 'X');
}

void getInput() {
    player.ox = player.x;
    player.oy = player.y;
    char ch = getC();
    if (ch == KEY_LEFT) {
        player.horzMove -= 1;
    } else if (ch == KEY_RIGHT) {
        player.horzMove += 1;
    } else if (ch == KEY_UP || ch == ' ') {
        player.vertMove += 2;
    } 
}

// int slogic(int tomovex, int tomovey) {
//     int xtomove = (player.horzMove > 0)? 1 : (player.horzMove < 0)? -1 : 0;
//     int ytomove = (player.vertMove > 0)? 1 : (player.vertMove < 0)? -1 : 0;
//     if ((walls.x <= player.x + xtomove && walls.x + walls.length >= player.x + xtomove) && walls.y == player.y + ytomove) {
//         if (player.y == walls.y) {
//             if (player.x < walls.x) {
//                 player.x = walls.x - 1;
//                 player.horzMove = (player.horzMove > 0)? player.horzMove * -1 : player.horzMove;
//             }
//             if (player.x > walls.x + walls.length) {
//                 player.x = walls.x + walls.length + 1;
//                 player.horzMove = (player.horzMove < 0)? player.horzMove * -1 : player.horzMove;
//             }
//         } else if (player.y > walls.y) {
//             player.y = walls.y + 1;
//             player.vertMove = 0;
//         } else if (player.y < walls.y) {
//             player.y = walls.y - 1;
//             player.vertMove = 0;
//         }
//         return -1;
//     }

//     int ret = 0;
//     if (tomovex > 0) {
//         player.x += xtomove;
//         ret ++;
//     }
//     if (tomovey > 0) {
//         player.y += ytomove;
//         ret += 10;
//     }

//     return ret;
// }

int Logic(int tick) {
    // int tomovex = abs(player.horzMove), tomovey = abs(player.vertMove);
    // for (int i = 0; i < (abs(player.horzMove) > abs(player.vertMove))? player.horzMove : player.vertMove; i++) {
    //     int ret = slogic(tomovex, tomovey);
    //     if (ret == -1) {
    //         break;
    //     }
    //     tomovex --; tomovey --;
    // }

    // int pas = 0;
    // if ((walls.x <= player.x + player.horzMove && walls.x + walls.length >= player.x + player.horzMove) && walls.y == player.y + player.vertMove) {
    //     if (player.y == walls.y) {
    //         if (player.x < walls.x) {
    //             player.x = walls.x - 1;
    //             player.horzMove = (player.horzMove > 0)? player.horzMove * -1 : player.horzMove;
    //         }
    //         if (player.x > walls.x + walls.length) {
    //             player.x = walls.x + walls.length + 1;
    //             player.horzMove = (player.horzMove < 0)? player.horzMove * -1 : player.horzMove;
    //         }
    //     } else if (player.y > walls.y) {
    //         player.y = walls.y + 1;
    //         player.vertMove = 0;
    //     } else if (player.y < walls.y) {
    //         player.y = walls.y - 1;
    //         player.vertMove = 0;
    //     }
    //     pas = 1;
    // }
    // if (pas == 0) {
        player.x += player.horzMove;
        player.y += player.vertMove;
    // }

    if (tick % 15 == 0)
        player.horzMove -= (player.horzMove > 0)? 1 : (player.horzMove < 0)? -1 : 0;

    if (player.horzMove >= 5) {
        player.horzMove = 5;
    } else if (player.vertMove >= 2) {
        player.vertMove = 2;
    }

    if (player.x == enemy.x && player.y == enemy.y)
        return -1;
    if (player.y <= 0) {
        player.y = 0;
        player.vertMove = 0;
    } else if (player.y >= 22) {
        player.y = 22;
        player.vertMove = (player.vertMove > 0)? player.vertMove * -1 : player.vertMove;
    } else { // if (player.vertMove != 0 || (player.y != walls.y + 1 && (player.x >= walls.x && player.x <= walls.x + walls.length)))
        player.vertMove -= 2;
        if (player.y >= 4) {
            player.vertMove -= 2;
        }
    }
    if (player.x <= 0) {
        player.x = 0;
        player.horzMove = (player.horzMove < 0)? player.horzMove * -1 : player.horzMove;
    } else if (player.x >= 77) {
        player.x = 77;
        player.horzMove = (player.horzMove > 0)? player.horzMove * -1 : player.horzMove;
    }
    return 0;
}