/*
MIT License

Copyright (c) 2023 Gracie Jackson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "funcs.h"

const static size_t NUM_COLS = 80;
const static size_t NUM_ROWS = 25;

struct Char {
    uint8_t character;
    uint8_t color;
};

struct Char* buffer = (struct Char*) 0xb8000;
size_t col = 0;
size_t row = 0;
uint8_t color = PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4;

void clear_row(size_t row) {
    struct Char empty = (struct Char) {
        character: ' ',
        color: color,
    };

    for (size_t col = 0; col < NUM_COLS; col ++) {
        buffer[col + NUM_COLS * row] = empty;
    }
}

void print_clear() {
    for (size_t i = 0; i < NUM_ROWS; i++) {
        clear_row(i);
    }
}

void print_newline() {
    col = 0;
    if (row < NUM_ROWS - 1) {
        row ++;
        return;
    }

    for (size_t row = 1; row < NUM_ROWS; row ++) {
        for (size_t col = 0; col < NUM_COLS; col ++) {
            struct Char character = buffer[col + NUM_COLS * row];
            buffer[col + NUM_COLS * (row-1)] = character;
        }
    }

    clear_row(NUM_ROWS - 1);
}

void print_char(char character) {
    if (character == '\n') {
        print_newline();
        return;
    }

    if (col > NUM_COLS) {
        print_newline();
    }

    buffer[col + NUM_COLS * row] = (struct Char) {
        character: (uint8_t) character,
        color: color,
    }; 

    col ++;
}

void print_str(char *str) {
    for (size_t i = 0; 1; i++) {
        char character = (uint8_t) str[i];
        
        if (character == '\0') {
            return;
        }

        print_char(character);
    }
}

void print_int(int num) {
    if (num == 0) { print_char('0'); return; }
    char toprint[20];
    int i = 0;
    while (num != 0) {
        toprint[i] = (num%10) + 48;
        num -= num%10;
        num /= 10;
        i ++;
    }
    for (int j = i-1; j > -1; j--) {
        print_char(toprint[j]);
    }
}

void print_set_color(uint8_t foreground, uint8_t background) {
    color = foreground + (background << 4);
}

void MSDelay(unsigned int itime) {
    unsigned int i,j;
    for (i=0;i<itime;i++) {
        for (j=0;j<1275;j++) asm volatile("nop");
    }
}

unsigned short lfsr = 0xACE1u;
unsigned int bit;

unsigned int rand() {
    bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
    return lfsr = (lfsr >> 1) | (bit << 15);
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "d"(port));
    return ret;
}

char get_input_keycode() {
    char ch = 0;
    while((ch = inb(KEYBOARD_PORT)) != 0) {
        if(ch > 0)
        return ch;
    }
    return ch;
}

/*
keep the cpu busy for doing nothing(nop)
so that io port will not be processed by cpu
here timer can also be used, but lets do this in looping counter
*/
void wait_for_io(uint32_t timer_count) {
    while(1) {
        asm volatile("nop");
        timer_count --;
        if(timer_count <= 0)
            break;
    }
}

void sleep(uint32_t timer_count) {
    wait_for_io(timer_count);
}

char get_ascii_char(char keycode) {
    if (keycode == KEY_A) return 0x61;
    if (keycode == KEY_B) return 0x62;
    if (keycode == KEY_C) return 0x63;
    if (keycode == KEY_D) return 0x64;
    if (keycode == KEY_E) return 0x65;
    if (keycode == KEY_F) return 0x66;
    if (keycode == KEY_G) return 0x67;
    if (keycode == KEY_H) return 0x68;
    if (keycode == KEY_I) return 0x69;
    if (keycode == KEY_J) return 0x6A;
    if (keycode == KEY_K) return 0x6B;
    if (keycode == KEY_L) return 0x6C;
    if (keycode == KEY_M) return 0x6D;
    if (keycode == KEY_N) return 0x6E;
    if (keycode == KEY_O) return 0x6F;
    if (keycode == KEY_P) return 0x70;
    if (keycode == KEY_Q) return 0x71;
    if (keycode == KEY_R) return 0x72;
    if (keycode == KEY_S) return 0x73;
    if (keycode == KEY_T) return 0x74;
    if (keycode == KEY_U) return 0x75;
    if (keycode == KEY_V) return 0x76;
    if (keycode == KEY_W) return 0x77;
    if (keycode == KEY_X) return 0x78;
    if (keycode == KEY_Y) return 0x79;
    if (keycode == KEY_Z) return 0x7A;
    if (keycode == KEY_1) return 0x31;
    if (keycode == KEY_2) return 0x32;
    if (keycode == KEY_3) return 0x33;
    if (keycode == KEY_4) return 0x34;
    if (keycode == KEY_5) return 0x35;
    if (keycode == KEY_6) return 0x36;
    if (keycode == KEY_7) return 0x37;
    if (keycode == KEY_8) return 0x38;
    if (keycode == KEY_9) return 0x39;
    if (keycode == KEY_0) return 0x30;
    if (keycode == KEY_MINUS) return 0x2D;
    if (keycode == KEY_EQUAL) return 0x3D;
    if (keycode == KEY_SQUARE_OPEN_BRACKET) return 0x5B;
    if (keycode == KEY_SQUARE_CLOSE_BRACKET) return 0x5D;
    if (keycode == KEY_SEMICOLON) return 0x3B;
    if (keycode == KEY_BACKSLASH) return 0x5c;
    if (keycode == KEY_COMMA) return 0x2C;
    if (keycode == KEY_DOT) return 0x2E;
    if (keycode == KEY_FORESLHASH) return 0x2F;
    // Here up to
    if (keycode == KEY_F1) return 0x3B;
    if (keycode == KEY_F2) return 0x3C;
    if (keycode == KEY_F3) return 0x3D;
    if (keycode == KEY_F4) return 0x3E;
    if (keycode == KEY_F5) return 0x3F;
    if (keycode == KEY_F6) return 0x40;
    if (keycode == KEY_F7) return 0x41;
    if (keycode == KEY_F8) return 0x42;
    if (keycode == KEY_F9) return 0x43;
    if (keycode == KEY_F10) return 0x44;
    if (keycode == KEY_F11) return 0x85;
    if (keycode == KEY_F12) return 0x86;

    if (keycode == KEY_BACKSPACE) return 0x08;
    if (keycode == KEY_DELETE) return 0x7F;

    if (keycode == KEY_DOWN) return 0x50;
    if (keycode == KEY_END) return 0x4F;
    if (keycode == KEY_ENTER) return 0x1C;

    if (keycode == KEY_ESC) return 0x1B;

    if (keycode == KEY_HOME) return 0x47;
    if (keycode == KEY_INSERT) return 0x52;
    if (keycode == KEY_KEYPAD_5) return 0x4C;
    if (keycode == KEY_KEYPAD_MUL) return 0x37;
    if (keycode == KEY_KEYPAD_Minus) return 0x4A;
    if (keycode == KEY_KEYPAD_PLUS) return 0x4E;
    if (keycode == KEY_KEYPAD_DIV) return 0x35;
    if (keycode == KEY_LEFT) return 0x4B;
    if (keycode == KEY_PAGE_DOWN) return 0x51;
    if (keycode == KEY_PAGE_UP) return 0x49;
    if (keycode == KEY_PRINT_SCREEN) return 0x37;
    if (keycode == KEY_RIGHT) return 0x4D;
    if (keycode == KEY_SPACE) return 0x20;
    if (keycode == KEY_TAB) return 0x0F;
    if (keycode == KEY_UP) return 0x48;

    return 0;
}

char getC() {
    char ch, keycode;
    int i = 0;
    do{
        keycode = get_input_keycode();
        if(keycode != 0) {
            ch = get_ascii_char(keycode);
            return ch;
        }
        sleep(0x02FFFFFF);
        i ++;
    } while(i > 0);
    return 0;
}

char *input() {
    char keycode = 0;
    int point = 0;
    do{
        keycode = get_input_keycode();
        if(keycode == KEY_ENTER){
            Input[point] = '\n';
            Input[++ point] = 0;
            return Input;
        } if(keycode == KEY_ESC){
            Input[point] = 0;
            return Input;
        } else {
            Input[point] = get_ascii_char(keycode);
        }
        sleep(0x02FFFFFF);
        point ++;
    } while(point < 255);

    return Input;
}

void Prininput() {
    char ch = 0;
    char keycode = 0;
    do{
        keycode = get_input_keycode();
        if(keycode == KEY_ENTER){
            print_newline();
        } if(keycode == KEY_ESC){
            return;
        } else {
            ch = get_ascii_char(keycode);
            print_char(ch);
        }
        sleep(0x02FFFFFF);
    } while(ch > 0);
}

void kernel_entry() {
    print_str("Type here, one key per second, ENTER to go to next line");
    print_newline();
    Prininput();
}

int strcmp(char *s1, char *s2) {
    int len1 = 0, len2 = 0;
    for (len1; s1[len1] != 0; len1 ++);
    for (len2; s2[len2] != 0; len2 ++);

    if (len1 != len2) {
        return 0;
    }

    for (int i = 0; i < len1; i++) {
        if (s1[i] != s2[i] && s1[i]-32 != s2[i] && s1[i] != s2[i]-32 && s1[i]+32 != s2[i] && s1[i] != s2[i]+32) {
            return 0;
        }
    }

    return 1;
}

int ai(char board[3][3]);
int minimax(char board[3][3], int maximizingPlayer);
void printBoard(char board[3][3]);
int checkwin(char board[3][3]);

void TTTgameLoop() {
    char board[3][3] = { {' ', ' ', ' '},
                         {' ', ' ', ' '},
                         {' ', ' ', ' '} };
    printBoard(board);
    char in, player = 'X';
    int win, moved, bMove;
    while (1){
        moved = 0;
        if (player == 'X') {
            bMove = ai(board);
            board[(bMove - (bMove%3))/3][bMove%3] = 'X';
            player = 'O';
        } else {
            do {
            in = ((int)getC()) - 48;
            if (in >= 0 && in < 9) {
                if (board[(int)((in-in%3)/3)][in%3] == ' ') {
                    board[(int)((in-in%3)/3)][in%3] = player;
                    moved = 1;
                }
            }
            sleep(0x02FFFFFF);
            } while(moved == 0);
            player = 'X';
        }
        
        printBoard(board);
        win = checkwin(board);
        if (win == 1) 
            print_str("X wins!\n");
        else if (win == -1)
            print_str("O wins!\n");
        else if (win == 2)
            print_str("Tie!\n");
        if (win == 1 || win == -1 || win == 2)
            break;

        sleep(0x02FFFFFF);
    }
}

void printBoard(char board[3][3]) {
    print_clear();
    char nboard[3][3] = { {'0', '1', '2'},
                          {'3', '4', '5'},
                          {'6', '7', '8'}};
                          
    print_str("Tic Tac Toe\n\n");
    for (int i=0; i<3; i++) {
        print_str(" +---+---+---+\n");
        for (int j=0; j<3; j++) {
            print_str(" | ");
            print_char(nboard[i][j]);
        }
        print_str(" | \n");
    }
    print_str(" +---+---+---+\n");

    print_str("\n\n\n");

    sleep(0x02FFFFFF);
    print_str("Tic Tac Toe\n\n");
    for (int i=0; i<3; i++) {
        print_str(" +---+---+---+\n");
        for (int j=0; j<3; j++) {
            print_str(" | ");
            print_char(board[i][j]);
        }
        print_str(" | \n");
    }
    print_str(" +---+---+---+\n");
}

int checkwin(char board[3][3]) {
    int seen = 0, xrow = 0, orow = 0, xcol = 0, ocol = 0;
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            if (board[i][j] == ' ')
                seen ++;
            else if (board[i][j] == 'X' && orow == 0)
                xrow ++;
            else if (board[i][j] == 'O' && xrow == 0)
                orow ++;
            if (board[j][i] == 'X' && orow == 0)
                xcol ++;
            else if (board[j][i] == 'O' && xrow == 0)
                ocol ++;
        }
        if (xcol == 3 || xrow == 3)
            return 1;
        else if (ocol == 3 || orow == 3)
            return -1;
        xrow = 0, orow = 0, xcol = 0, ocol = 0;
    }
    
    if ((board[0][0] == 'X' && board[1][1] == 'X' && board[2][2] == 'X') || (board[2][0] == 'X' && board[1][1] == 'X' && board[0][2] == 'X'))
        return 1;
    else if ((board[0][0] == 'O' && board[1][1] == 'O' && board[2][2] == 'O') || (board[2][0] == 'O' && board[1][1] == 'O' && board[0][2] == 'O'))
        return -1;
    
    if (seen == 0)
        return 2;

    return 0;
}

int minimax(char board[3][3], int maximizingPlayer) {
    int result = checkwin(board);
    if (result == 1 || result == -1){
        return result;
    } if (result == 2){
        return 0;
    }
    if (maximizingPlayer == 1){
        int value = -800;
        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                if (board[i][j] == ' '){
                    board[i][j] = 'X';
                    int sol = minimax(board, 0);
                    board[i][j] = ' ';
                    if (value < sol){
                        value = sol;
                    }
                }
            }
        }
        return value;
    } else {
        int value = 800;
        for (int i = 0; i < 3; i++){
            for (int j = 0; j < 3; j++){
                if (board[i][j] == ' '){
                    board[i][j] = 'O';
                    int sol = minimax(board, 1);
                    board[i][j] = ' ';
                    if (value > sol){
                        value = sol;
                    }
                }
            }
        }
        return value;
    }
}

int ai(char board[3][3]) {
    int bestScore = -800;
    int bestMove = 0;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (board[i][j] == ' '){
                board[i][j] = 'X';
                int score = minimax(board, 0);
                // printw("\n%d",score);
                board[i][j] = ' ';
                if (score > bestScore){
                    bestScore = score;
                    bestMove = i*3+j;
                }
            }
        }
    }
    int bestmove = 0;
    int largest = -800;

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (board[i][j] == ' '){
                board[i][j] = 'X';
                int result = minimax(board, 0);
                // printw("\n%d",result);
                board[i][j] = ' ';
                if (result > largest){
                    largest = result;
                    bestmove = i*3+j;
                }
            }
        }
    }
    
    // getch();
    return bestMove;
}

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
        if (logic() == -1 || last == -1) { refresh(); break; }
        MSDelay(30000); // 100000
    }
    for (int i=score+1; i > -1; i--) {
        buffer[snakeX[i] + NUM_COLS * snakeY[i]] = (struct Char) { character: (uint8_t) ' ', color: color, };
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
                print_char('#');
            } else {
                print_char(' ');
            }
        }
    }
    print_char('\n');
}

void refresh() {
    struct Char empty = (struct Char) {
        character: ' ',
        color: color,
    };
    buffer[oX + NUM_COLS * oY] = empty;
    buffer[ofX + NUM_COLS * ofY] = empty;

    // buffer[snakeX[0]-1 + NUM_COLS * snakeY[0]] = (struct Char) { character: (uint8_t) ' ', color: color, };

    buffer[snakeX[0] + NUM_COLS * snakeY[0]] = (struct Char) {
        character: (uint8_t) 'O',
        color: color,
    };

    buffer[fruitX + NUM_COLS * fruitY] = (struct Char) {
        character: (uint8_t) '*',
        color: color,
    };

    for (int i=0; i < score; i++) {
        buffer[snakeX[i+1] + NUM_COLS * snakeY[i+1]] = (struct Char) {
            character: (uint8_t) 'o',
            color: color,
        }; 
    }
    int num = score;

    if (num == 0) { buffer[1] = (struct Char) { character: (uint8_t) '0', color: color, }; return; }
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
        buffer[k] = (struct Char) { character: (uint8_t) toprint[j], color: color, };
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
start:
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
            // goto start;
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
    if (snakeX[0] >= 78) {
        snakeX[0] = 2;
        // return -1;
    } 
    if (snakeY[0] >= 22) {
        snakeY[0] = 2;
        //return -1;
    }
    if (snakeX[0] <= 1) {
        snakeX[0] = 77;
        // return -1;
    } if (snakeY[0] <= 1) {
        snakeY[0] = 21;
        // return -1;
    }
    // for (int i=score+1; i > 0; i--) {
    //     if (snakeX[i] == snakeX[0] && snakeY[i] == snakeY[0]) {
    //         return -1;
    //     }
    // }
    return 0;
}


// CHESS
//

//            rnbqkbnr/pppppppp/11111111/11111111/11111111/11111111/PPPPPPPP/RNBQKBNR w KQkq
char FEN[] = "rnbqkbnrpppppppp11111111111111111111111111111111PPPPPPPPRNBQKBNR w KQkq";
int curs = 0, svcurs = 0, turn = 0;
uint8_t Black = PRINT_COLOR_WHITE | PRINT_COLOR_BLACK << 4;
uint8_t White = PRINT_COLOR_BLACK |  PRINT_COLOR_WHITE << 4;

int checkLegal(int old, int new, char piece, char oPiece);
char ChessgetIn(int num);
int Chesslogic();
void ChessDraw();
void ChessInit();

void ChessGameloop() {
    MSDelay(70000);
    ChessInit();
    MSDelay(70000);
    char cap;
    while (1) {
        ChessDraw();
        cap = ChessgetIn(0);
        if (Chesslogic() == -1) { ChessDraw(); break; }

        MSDelay(70000); // 100000
    }
}

void ChessInit() {
    // Box = 1,1 - 18,34
    print_clear();
    for (int i = 0; i<8; i++) {
        // 1,1 - 1,34 LINE
        // 2,1 - 2,34 FEN
        print_str("\n +---+---+---+---+---+---+---+---+ \n");
        for (int j = 0; j<8; j++) {
            print_str(" |  ");
        }
        print_str(" | ");
    }
    print_str("\n +---+---+---+---+---+---+---+---+");
    int bw = 1;
    for (int i=0; i<8; i++) {
        bw = (bw != 0)? 0:1;
        for (int j = 0; j<8; j++) {
            buffer[((i+1)*2+1)*NUM_COLS + (j+1)*4-2] = (struct Char) { character: (uint8_t) ' ', color: (bw == 0)? Black:White, };
            buffer[((i+1)*2+1)*NUM_COLS + (j+1)*4] = (struct Char) { character: (uint8_t) ' ', color: (bw == 0)? Black:White, };
            bw = (bw != 0)? 0:1;
        }
    }
}

void ChessDraw() {
    int bw = 1;
    for (int i=0; i<8; i ++) {
        bw = (bw != 0)? 0:1;
        for (int j=0; j<8; j++) {
            if (i*8+j == curs) {
                buffer[((i+1)*2+1)*NUM_COLS + (j+1)*4-2] = (struct Char) { character: (uint8_t) '#', color: (bw == 0)? Black:White, };
            } else if (i*8+j == svcurs) {
                buffer[((i+1)*2+1)*NUM_COLS + (j+1)*4-2] = (struct Char) { character: (uint8_t) ' ', color: (bw == 0)? Black:White, };
            }
            if (FEN[i*8 + j] == '1') {
                buffer[((i+1)*2+1)*NUM_COLS + (j+1)*4-1] = (struct Char) { character: (uint8_t) ' ', color: (bw == 0)? Black:White, };
            } else {
                buffer[((i+1)*2+1)*NUM_COLS + (j+1)*4-1] = (struct Char) { character: (uint8_t) FEN[i*8+j], color: (bw == 0)? Black:White, };
            }
            bw = (bw != 0)? 0:1;
        }
    }
}

char ChessgetIn(int num) {
    svcurs = curs;
    char in = getC();
    if (in == KEY_LEFT) 
        curs --;
    else if (in == KEY_RIGHT)
        curs ++;
    else if (in == KEY_UP)
        curs -= 8;
    else if (in == KEY_DOWN)
        curs += 8;
    else if (in == 0x1C && num == 0) {
        char stor = FEN[curs];
        if (stor == '1')
            return in;
        int storcur = curs;
        FEN[curs] = '1';
        ChessDraw();
        MSDelay(100000);
        while (ChessgetIn(1) != 0x1C) { Chesslogic(0); ChessDraw(); MSDelay(70000); }
        // 0x61 and 0x79
        if (checkLegal(storcur, curs, stor, FEN[curs]) != 0) {
            FEN[curs] = stor;
            turn = (turn == 0)? 1 : 0;
        }
        else { FEN[storcur] = stor; }
    }

    return in;
}

int checkLegal(int old, int new, char piece, char oPiece) {
    if (turn == 0 && piece <= 0x61) {
        return 0;
    } else if (turn == 1 && piece >= 0x61) {
        return 0;
    }
    if (oPiece != '1' && ((piece <= 0x61  && oPiece <= 0x61) || (piece >= 0x61 &&  oPiece >= 0x61))) {
        return 0;
    }

    if (old == new) {
        return 0;
    }
    if (piece == 'p') {
        if ((old + 8 == new && oPiece == '1') || (old + 16 == new && (old - old%8)/8 == 1 && FEN[old+8] == '1' && oPiece == '1') || (oPiece != '1' && (old + 9 == new || old + 7 == new))) {
            if ((new - new%8)/8 == 7)
                FEN[new] = 'q';
            return 1;
        }
        return 0;
    } else if (piece == 'P') {
        if ((old - 8 == new && oPiece == '1') || (old - 16 == new && (old - old%8)/8 == 6 && FEN[old-8] == '1' && oPiece == '1') || (oPiece != '1' && (old - 9 == new || old - 7 == new))) {
            if ((new - new%8)/8 == 0)
                FEN[new] = 'Q';
            return 1;
        }
        return 0;
    }
    if (piece == 'r' || piece == 'R') {
        int dif = new - old;
        if (dif >= 8 || dif <= -8) {
            if (old%8 != new%8)
                return 0;
            int orow = (old - old%8) / 8, nrow = (new - new%8) / 8;
            if (orow < nrow)
                for (int i=orow+1; i < nrow; i++) {
                    if (FEN[i*8+old%8] != '1')
                        return 0;
                }
            else
                for (int i=orow-1; i > nrow; i--) {
                    if (FEN[i*8+old%8] != '1')
                        return 0;
                }
            return 1;
        } else {
            int ocol = old%8, ncol = new%8;
            if ((old - old%8) / 8 != (new - new%8) / 8)
                return 0;

            if (ocol < ncol) {
                for (int i=ocol+1; i < ncol; i++) {
                    if (FEN[i + (old-old%8)] != '1') {
                        return 0;
                    }
                }
            } else {
                for (int i=ocol-1; i > ncol; i--) {
                    if (FEN[i+(old-old%8)] != '1') {
                        return 0;
                    }
                }
            }
            return 1;
        }
        return 0;
    }
    if (piece == 'k' || piece == 'K') {
        if (old+1 == new || old-1 == new || old+8 == new || old-8 == new || old + 7 == new || old - 7 == new || old + 9 == new || old - 9 == new)
            return 1; // 67 68 Black 69 70 for White
        // else if (old + 2 == new && FEN[67] != ' ' && FEN[62] == ' ' && FEN[61] == ' ' && FEN[63] == 'R' && (old - old%8)/8 == 7 && piece == 'K') {
        //     FEN[63] = '1';
        //     FEN[67] = ' ';
        //     FEN[61] = 'R';
        //     return 1;
        // } else if (old + 2 == new && FEN[69] != ' ' && FEN[6] == ' ' && FEN[5] == ' ' && FEN[7] == 'r' && (old - old%8)/8 == 0 && piece == 'k') {
        //     FEN[7] = '1';
        //     FEN[69] = ' ';
        //     FEN[5] = 'R';
        //     return 1;
        // }
        return 0;
    }
    if (piece == 'n' || piece == 'N') {
        if (old-19 == new || old-17 == new || old-15 == new || old-10 == new || old-6 == new  || old+19 == new || old+17 == new || old+15 == new || old+10 == new || old+6 == new)
            return 1;
        return 0;
    }
    if (piece == 'b' || piece == 'B') {
        if (old % 8 < new % 8) {
            if ((old - old%8)/8 < (new - new%8)/8) {
                for (int i = 0; i < new%8 - old%8; i++) {
                    if (FEN[old + i + i*8] != '1') {
                        return 0;
                    }
                }
                if ((old + (new%8 - old%8) + (new%8  - old%8)*8) != new) {
                    return 0;
                }
                return 1;
            } else if ((old - old%8)/8 > (new - new%8)/8) {
                for (int i = 0; i < new%8 - old%8; i++) {
                    if (FEN[old + i - i*8] != '1') {
                        return 0;
                    }
                }
                if ((old + (new%8 - old%8) - (new%8  - old%8)*8) != new){
                    return 0;
                }
                return 1;
            } 
        } else if (old % 8 > new % 8) {
            if ((old - old%8)/8 < (new - new%8)/8) {
                for (int i = 0; i < old%8 - new%8; i++) {
                    if (FEN[old - i + i*8] != '1') {
                        return 0;
                    }
                }
                if ((old - (old%8 - new%8) + (old%8 - new%8)*8) != new){
                    return 0;
                }
                return 1;
            } else if ((old - old%8)/8 > (new - new%8)/8) {
                for (int i = 0; i < old%8 - new%8; i++) {
                    if (FEN[old - i - i*8] != '1') {
                        return 0;
                    }
                }
                if ((old - (old%8 - new%8) - (old%8 - new%8)*8) != new){
                    return 0;
                }
                return 1;
            } 
        }
        return 0;
    }
    if (piece == 'q' || piece == 'Q') {
        if (new % 8 == old % 8 || (new - new % 8) == (old - old % 8)) {
            int dif = new - old;
            if (dif >= 8 || dif <= -8) {
                if (old%8 != new%8)
                    return 0;
                int orow = (old - old%8) / 8, nrow = (new - new%8) / 8;
                if (orow < nrow)
                    for (int i=orow+1; i < nrow; i++) {
                        if (FEN[i*8+old%8] != '1')
                            return 0;
                    }
                else
                    for (int i=orow-1; i > nrow; i--) {
                        if (FEN[i*8+old%8] != '1')
                            return 0;
                    }
                return 1;
            } else {
                int ocol = old%8, ncol = new%8;
                if ((old - old%8) / 8 != (new - new%8) / 8)
                    return 0;

                if (ocol < ncol) {
                    for (int i=ocol+1; i < ncol; i++) {
                        if (FEN[i + (old-old%8)] != '1') {
                            return 0;
                        }
                    }
                } else {
                    for (int i=ocol-1; i > ncol; i--) {
                        if (FEN[i+(old-old%8)] != '1') {
                            return 0;
                        }
                    }
                }
                return 1;
            }
        } else {
            if (old % 8 < new % 8) {
                if ((old - old%8)/8 < (new - new%8)/8) {
                    for (int i = 0; i < new%8 - old%8; i++) {
                        if (FEN[old + i + i*8] != '1') {
                            return 0;
                        }
                    }
                    if ((old + (new%8 - old%8) + (new%8  - old%8)*8) != new) {
                        return 0;
                    }
                    return 1;
                } else if ((old - old%8)/8 > (new - new%8)/8) {
                    for (int i = 0; i < new%8 - old%8; i++) {
                        if (FEN[old + i - i*8] != '1') {
                            return 0;
                        }
                    }
                    if ((old + (new%8 - old%8) - (new%8  - old%8)*8) != new){
                        return 0;
                    }
                    return 1;
                } 
            } else if (old % 8 > new % 8) {
                if ((old - old%8)/8 < (new - new%8)/8) {
                    for (int i = 0; i < old%8 - new%8; i++) {
                        if (FEN[old - i + i*8] != '1') {
                            return 0;
                        }
                    }
                    if ((old - (old%8 - new%8) + (old%8 - new%8)*8) != new){
                        return 0;
                    }
                    return 1;
                } else if ((old - old%8)/8 > (new - new%8)/8) {
                    for (int i = 0; i < old%8 - new%8; i++) {
                        if (FEN[old - i - i*8] != '1') {
                            return 0;
                        }
                    }
                    if ((old - (old%8 - new%8) - (old%8 - new%8)*8) != new){
                        return 0;
                    }
                    return 1;
                } 
            }
        }
        return 0;
    }

    return 0;
}

int Chesslogic() {
    if (curs <= 0) curs = 0;
    else if (curs >= 63) curs = 63;
    int seenb = 0, seenw = 0;

    for (int i=0; i < 64; i++) {
        if (FEN[i] == 'k') {
            seenw ++;
        } else if (FEN[i] == 'K') {
            seenb ++;
        }
    }

    if (seenb == 0 || seenw == 0) {
        return -1;
    }
    return 0;
}

