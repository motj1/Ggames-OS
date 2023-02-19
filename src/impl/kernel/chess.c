#include "funcs.h"

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
    print_char('\n');
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
            printPos((j+1)*4-2, (i+1)*2+1, (bw == 0)? Black:White, ' ');
            printPos((j+1)*4, (i+1)*2+1, (bw == 0)? Black:White, ' ');
            // buffer[((i+1)*2+1)*NUM_COLS + (j+1)*4-2] = (struct Char) { character: (uint8_t) ' ', color: (bw == 0)? Black:White, };
            // buffer[((i+1)*2+1)*NUM_COLS + (j+1)*4] = (struct Char) { character: (uint8_t) ' ', color: (bw == 0)? Black:White, };
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
                printPos((j+1)*4-2, (i+1)*2+1, (bw == 0)? Black:White, '#');
                // buffer[((i+1)*2+1)*NUM_COLS + (j+1)*4-2] = (struct Char) { character: (uint8_t) '#', color: (bw == 0)? Black:White, };
            } else if (i*8+j == svcurs) {
                printPos((j+1)*4-2, (i+1)*2+1, (bw == 0)? Black:White, ' ');
                // buffer[((i+1)*2+1)*NUM_COLS + (j+1)*4-2] = (struct Char) { character: (uint8_t) ' ', color: (bw == 0)? Black:White, };
            }
            if (FEN[i*8 + j] == '1') {
                printPos((j+1)*4-1, (i+1)*2+1, (bw == 0)? Black:White, ' ');
                // buffer[((i+1)*2+1)*NUM_COLS + (j+1)*4-1] = (struct Char) { character: (uint8_t) ' ', color: (bw == 0)? Black:White, };
            } else {
                printPos((j+1)*4-1, (i+1)*2+1, (bw == 0)? Black:White, FEN[i*8+j]);
                // buffer[((i+1)*2+1)*NUM_COLS + (j+1)*4-1] = (struct Char) { character: (uint8_t) FEN[i*8+j], color: (bw == 0)? Black:White, };
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