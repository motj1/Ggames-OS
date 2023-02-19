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
    col = 0;
    row = 0;
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

void print_char_color(char character, uint8_t colour) {
    if (character == '\n') {
        print_newline();
        return;
    }

    if (col > NUM_COLS) {
        print_newline();
    }

    buffer[col + NUM_COLS * row] = (struct Char) {
        character: (uint8_t) character,
        color: colour,
    }; 

    col ++;
}

void printPos(int x, int y, uint8_t colour, char character) {
    buffer[x + NUM_COLS * y] = (struct Char) {
        character: (uint8_t) character,
        color: colour,
    };
}

void print_str_color(char *str, uint8_t colour) {
    for (size_t i = 0; 1; i++) {
        char character = (uint8_t) str[i];
        
        if (character == '\0') {
            return;
        }

        print_char_color(character, colour);
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

void outb(uint16_t port, uint8_t data) {
    asm volatile("outb %1, %0" : : "dN" (port), "a"(data));
}

void cli() {
    asm ("cli");
}

unsigned read_pit_count(void) {
    unsigned count = 0;

    // Disable interrupts
    cli();

    // al = channel in bits 6 and 7, remaining bits clear
    outb(0x43,0b0000000);

    count = inb(0x40);		// Low byte
    count |= inb(0x40)<<8;		// High byte

    return count;
}

char get_input_keycode() {
    char ch = 0;
    while((ch = inb(KEYBOARD_PORT)) != 0) {
        if(ch > 0)
            return ch;
    }
    return ch;
}

char get_input_keycode_time(uint32_t time) {
    char ch = 0;
    unsigned int i = 3;
    outb(0x20, 0x20);
    while((ch = inb(KEYBOARD_PORT)) != 0) {
        if(ch > 0)
            return ch;
        return 0;
        i ++;
        if (i > time * 1275)
            return 0;
        outb(KEYBOARD_PORT, KEY_UP);
    }
    return ch;
}

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

char getC_time(uint32_t time) {
    char ch, keycode;
    int i = 0;
    do{
        keycode = get_input_keycode_time(time);
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


//Play sound using built in speaker
static void play_sound(uint32_t nFrequence) {
    uint32_t Div;
    uint8_t tmp;

        //Set the PIT to the desired frequency
    Div = 1193180 / nFrequence;
    outb(0x43, 0xb6);
    outb(0x42, (uint8_t) (Div) );
    outb(0x42, (uint8_t) (Div >> 8));

        //And play the sound using the PC speaker
    tmp = inb(0x61);
    if (tmp != (tmp | 3)) {
        outb(0x61, tmp | 3);
    }
}

//make it shutup
static void nosound() {
    uint8_t tmp = inb(0x61) & 0xFC;

    outb(0x61, tmp);
}

//Make a beep
void beep() {
    play_sound(1000);
    MSDelay(10000);
    nosound();
        //set_PIT_2(old_frequency);
}
