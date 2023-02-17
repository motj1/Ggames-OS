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

void kernel_main() {
    MSDelay(1000);

    print_clear();
    print_set_color(PRINT_COLOR_YELLOW, PRINT_COLOR_BLACK);

    char *inp;

    /* while (1) {
        print_clear();
        print_str("Welcome to Gracie's games!\n");

        MSDelay(10000); // 100000

        char in;

        while (1) {
            print_str("> ");
            char *inp = input();

            if (strcmp(inp, "help") || strcmp(inp, "help\n")) {
                print_str(inp);
                print_str("\nNo help for you lol\n");
                return;
            } else if (strcmp(inp, "TTT") || strcmp(inp, "TTT\n")) {
                do {
                    print_clear();

                    TTTgameLoop();

                    MSDelay(100000);
                    print_str("Press any key to replay or q to quit\n");
                    in = getC();
                } while (in != 'q');
                break;
            } else if (strcmp(inp, "snake") || strcmp(inp, "snake\n")) {
                do {
                    print_clear();

                    SnakegameLoop();

                    MSDelay(100000); // 1000000
                    print_clear();
                    print_str("Press any key to replay or q to quit\n");
                    in = getC();
                } while (in != 'q');
                break;
            } else if (strcmp(inp, "chess") || strcmp(inp, "chess\n")) {
                do {
                    print_clear();

                    ChessGameloop();

                    MSDelay(100000); // 1000000
                    print_clear();
                    print_str("Press any key to replay or q to quit\n");
                    in = getC();
                } while (in != 'q');
                break;
            } else {
                print_str(inp);
                print_char('\n');
            }
            MSDelay(1000000);
        }
        print_clear();
        print_str("Play again? ");
        inp = input();
        print_str(inp);

        if (strcmp(inp, "n") || strcmp(inp, "no") || strcmp(inp, "qn") || strcmp(inp, "nn") || strcmp(inp, "no\n") || strcmp(inp, "n\n") || strcmp(inp, "qn\n") || strcmp(inp, "qno\n")) {
            break;
        } else if (strcmp(inp, "hlt") || strcmp(inp, "hlt\n") || strcmp(inp, "qhlt\n"))
            return;
    }
    print_clear();
    while (1) {
        MSDelay(100000);
        print_str("You are past the games!\n");
        while (1) {
            print_str("> ");
            inp = input();
            print_str(inp);

            if (strcmp(inp, "help")) {
                print_str("\nNo help for you\n");
                return;
            } else if (strcmp(inp, "hlt") || strcmp(inp, "hlt\n"))
                return;
            MSDelay(1000000);
        }
    }  */
    
    PlatformerGameLoop();
}