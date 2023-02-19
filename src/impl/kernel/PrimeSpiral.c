#include "funcs.h"

// int Sh = 25;
// int Sw = 40;

int lenOfPrimeSpiral = 40; // 26

int sieve[1600]; // 676

uint8_t notprime = PRINT_COLOR_DARK_GRAY | PRINT_COLOR_DARK_GRAY << 4;
uint8_t prime = PRINT_COLOR_YELLOW | PRINT_COLOR_YELLOW << 4;

void findPrimes();
void drawSpiral();

void spiralMainLoop() {
    print_clear();

    findPrimes();
    drawSpiral();
    
    MSDelay(1000000);
}

void drawSpiral() {
    int iter = 0, posx = 20, posy = 12, direction = 0;
    for (int length = 1; length < lenOfPrimeSpiral; length ++) {
        for (int i=0; i<2; i++) {
            for (int j=0; j<length; j++) {
                if (sieve[iter] == 1) {
                    printPos(posx*2, posy, prime, ' ');
                    printPos(posx*2+1, posy, prime, ' ');
                } else {
                    printPos(posx*2, posy, notprime, ' ');
                    printPos(posx*2+1, posy, notprime, ' ');
                }
                switch (direction)
                {
                case 0:
                    posx ++;
                    break;
                case 1:
                    posy ++;
                    break;
                case 2:
                    posx --;
                    break;
                case 3:
                    posy --;
                    break;
                default:
                    break;
                }
                iter ++;
                MSDelay(10000);
            }
            direction ++;
            direction %= 4;
        }
    }
}

void findPrimes() {
    sieve[0] = 0;
    sieve[1] = 0;
    for (int i=2; i < 1600; i++) {
        sieve[i] = 1;
    }
    for(int n = 2; n*n < 1600; n ++) {
        if(sieve[n] == 1) {
            for(int index = n*n; index < 1600; index += n) {
                sieve[index] = 0;
            }
        }
    }
}