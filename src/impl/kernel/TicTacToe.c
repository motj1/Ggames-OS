#include "funcs.h"

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