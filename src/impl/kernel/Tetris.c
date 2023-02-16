#include "funcs.h"

void TetrisInit();
void TetrisInput();
void TetrisdrawWindow();
void progress();

uint8_t normal = PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4;
uint8_t border = PRINT_COLOR_LIGHT_GRAY | PRINT_COLOR_LIGHT_GRAY << 4;
uint8_t red = PRINT_COLOR_RED | PRINT_COLOR_RED << 4;
uint8_t cyan = PRINT_COLOR_LIGHT_CYAN | PRINT_COLOR_LIGHT_CYAN << 4;
uint8_t yellow = PRINT_COLOR_YELLOW | PRINT_COLOR_YELLOW << 4;
uint8_t purple = PRINT_COLOR_MAGENTA | PRINT_COLOR_MAGENTA << 4;
uint8_t green = PRINT_COLOR_GREEN | PRINT_COLOR_GREEN << 4;
uint8_t orange = PRINT_COLOR_BROWN | PRINT_COLOR_BROWN << 4;
uint8_t blue = PRINT_COLOR_BLUE | PRINT_COLOR_BLUE << 4;

char S[4][1] = { {'c'}, {'c'}, {'c'}, {'c'} };
char L[3][2] = { {'b', ' '}, {'b', ' '}, {'b', 'b'} };
char Q[2][2] = { {'y', 'y'}, {'y', 'y'} };
char T[2][3] = { {' ', 'p', ' '}, {'p', 'p', 'p'} };

char board[20][10] = { "          ",
                       "          ",
                       "          ",
                       "          ",
                       "          ",
                       "          ",
                       "          ",
                       "          ",
                       "          ",
                       "          ",
                       "          ",
                       "          ",
                       "          ",
                       "          ",
                       "          ",
                       "          ",
                       "          ",
                       "          ",
                       "          ",
                       "          " };


void tetrisMainLoop() {
    MSDelay(70000);
    TetrisInit();
    // MSDelay(70000);
    char cap;
    while (1) {
        TetrisInput();
        TetrisdrawWindow();
        progress();
        MSDelay(70000);
    }
}

void TetrisInit() {
    print_clear();
    print_str_color("############\n", border);
    for (int i=0; i < 20; i++) {
        print_char_color('#', border);
        for (int j=0; j<10; j++) {
            print_char(' ');
        }
        print_str_color("#\n", border);
    }
    print_str_color("############\n", border);
}

void TetrisdrawWindow() {
    for (int i=1; i < 21; i++) {
        for (int j=1; j<11; j++) {
            if (board[i-1][j-1] == ' ') {
                // printPos(j, i, ' ', normal);
                continue;
            } else if (board[i-1][j-1] == 'r') {
                printPos(j, i, '#', red);
            } else if (board[i-1][j-1] == 'c') {
                printPos(j, i, '#', cyan);
            } else if (board[i-1][j-1] == 'y') {
                printPos(j, i, '#', yellow);
            } else if (board[i-1][j-1] == 'p') {
                printPos(j, i, '#', purple);
            } else if (board[i-1][j-1] == 'g') {
                printPos(j, i, '#', green);
            } else if (board[i-1][j-1] == 'o') {
                printPos(j, i, '#', orange);
            } else if (board[i-1][j-1] == 'b') {
                printPos(j, i, '#', blue);
            }
        }
    }
}

void TetrisInput() {
    char in = getC_time(3000);

    if (in == KEY_RIGHT) {
        for (int i=10; i > 0; i--) {
            for (int j=0; j<19; j++) {
                board[j][i] = board[j][i-1];
            }
        }
    } else if (in == 's') {
        for (int i=0; i<4; i++) {
            board[i+5][0] = S[i][0];
        }
    }
}

void progress() {
    for (int j=0; j<10; j++) {
        board[0][j] = board[19][j];
    }
    for (int i=19; i > 0; i--) {
        for (int j=0; j<10; j++) {
            board[i][j] = board[i-1][j];
        }
    }
}