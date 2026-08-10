//           DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//                    Version 2, December 2004
// 
// Copyright (C) 2004 Sam Hocevar
//  14 rue de Plaisance, 75014 Paris, France
// Everyone is permitted to copy and distribute verbatim or modified
// copies of this license document, and changing it is allowed as long
// as the name is changed.
// 
//            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
// 
//  0. You just DO WHAT THE FUCK YOU WANT TO.
//

#include <SPI.h>
#include <Gamebuino.h>

Gamebuino gb = Gamebuino();

// Helper for drawing the logo
#define PX00 B00000000
#define PX01 B00000110
#define PX10 B01100000
#define PX11 B01100110
#define EMPTYL B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000

// Maximum board sizes
#define XMAX 21
#define YMAX 12
#define RULES 4

// Bitmaps for lights (on, off, superstate)
const byte PROGMEM field[3][6] =
    { {
        4, 4,
        PX00,
        PX10,
        PX10,
        PX00
    } ,{
        4, 4,
        PX00,
        PX00,
        PX00,
        PX00,
    } ,{
        4, 4,
        B01100000,
        B10010000,
        B10010000,
        B01100000
    } };

// Logo
// This feels nasty, but I'm not sure how to do this properly.
const byte PROGMEM screenL[] =
    {
        56, 26,
        EMPTYL,
        EMPTYL,
        EMPTYL,
        EMPTYL,
        EMPTYL,
        EMPTYL,
        EMPTYL,
        PX00, PX11, PX00, PX10, PX00, PX10, PX01,
        PX00, PX11, PX00, PX10, PX00, PX10, PX01,
        EMPTYL,
        EMPTYL,
        PX01, PX00, PX10, PX10, PX10, PX10, PX10,
        PX01, PX00, PX10, PX10, PX10, PX10, PX10,
        EMPTYL,
        EMPTYL,
        PX00, PX11, PX10, PX11, PX10, PX11, PX00,
        PX00, PX11, PX10, PX11, PX10, PX11, PX00,
        EMPTYL,
        EMPTYL,
        PX00, PX00, PX10, PX00, PX10, PX10, PX10,
        PX00, PX00, PX10, PX00, PX10, PX10, PX10,
        EMPTYL,
        EMPTYL,
        PX00, PX11, PX00, PX00, PX10, PX10, PX01,
        PX00, PX11, PX00, PX00, PX10, PX10, PX01,
        EMPTYL
    };

// Board configuration for logo
byte logo[5][14] =
    {
        {1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0},
        {1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1},
        {1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1},
        {1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1},
        {1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 0}
    };


char* ruleSet[] = {"Classic", "Alt.", "Threeway", "SuperPos"};
char* toggleS[] = {"Off", "On"};

// Toggles a given light and the lights around it.
void toggle(char* board, int x, int y, int px, int py, int rule) {
    if(rule < 2) {
        if(px > 0) board[py * x + px - 1] ^= 1;
        if(px < x - 1) board[py * x + px + 1] ^= 1;
        if(py > 0) board[(py - 1) * x + px] ^= 1;
        if(py < y - 1) board[(py + 1) * x + px] ^= 1;
        if(rule == 0) board[py * x + px] ^= 1;
    } else {
        if(px > 0) board[py * x + px - 1] = (board[py * x + px - 1] + 1) % 3;
        if(px < x - 1) board[py * x + px + 1] = (board[py * x + px + 1] + 1) % 3;
        if(py > 0) board[(py - 1) * x + px] = (board[(py - 1) * x + px] + 1) % 3;
        if(py < y - 1) board[(py + 1) * x + px] = (board[(py + 1) * x + px] + 1) % 3;
        board[py * x + px] = (board[py * x + px] + 1) % 3;
    }
}

// Creates a new board with the dimensions x * y
char* newBoard(int x, int y, int rule) {
    char* board = (char *) malloc(x * y);
    // Init board
    for(int i = 0; i < y * x; i++) board[i] = 0;
    if(rule == 3) {
        // Set random fields to super position,
        // the board remains solvable by doing so.
        for(int i = 0; i < x * y; i++) {
            if(random(0,3) == 1) board[i] = 2;
        }
    }
    // Randomize board
    for(int i = 0; i < y; i++) {
        for(int k = 0; k < x; k++) {
            if(random(0,3) == 1) toggle(board, x, y, k, i, rule);
        }
    }
    return board;
}

// Intro animation
void introBoard() {
    int steps = 10;
    int togglesX[steps];
    int togglesY[steps];
    char* board = (char *) malloc(21 * 12);
    // Top non-logo area
    int i, x ,y, h, k;
    for(i = 0; i < 126; i++) board[i] = 1;
    // Set logo
    for(h = 0; h < 5; h++) {
        for(k = 0; k < 14; k++) board[i++] = logo[h][k];
        // Right non-logo area
        for(k = 0; k < 7; k++) board[i++] = 1;
    }
    // Fill the rest
    while(i < 12 * 21) board[i++] = 1;
    i = 0;
    // Randomize board
    for(i = 0; i < steps; i++) {
        // Stay near the logo
        x = random(0, 14);
        y = random(5, 12);
        toggle(board, 21, 12, x, y, 0);
        togglesX[i] = x;
        togglesY[i] = y;
    }
    // Gradually uncover logo
    i = 0; h = 0;
    while(i < steps) {
        if(gb.update()) {
            draw(board, 21, 12, 0, 0);
            if(h++ > 2) {
                h = 0;
                toggle(board, 21, 12, togglesX[i], togglesY[i], 0);
                gb.sound.playTick(); i++;
            }
            if(gb.buttons.pressed(BTN_A)) break;
        }
    }
    // Let it linger for a while
    i = 0;
    while(i < 3) {
        if(gb.update()) {
            draw(board, 21, 12, 0, 0);
            i++;
        }
    }
    free(board);
}

// Checks if the player won and displays a message if they did.
bool won(char* board, int x, int y, int rule, int startT, int toggles) {
    int time = gb.frameCount - startT;
    for(int i = 0; i < y * x; i++) {
        if(rule < 3) {
            if(board[i] != 0) return false;
        } else {
            if(board[i] == 1) return false;
        }
    }
    gb.sound.playOK();
    while(1) {
        if(gb.update()) {
            gb.display.cursorX = 6;
            gb.display.cursorY = 5;
            gb.display.print("You won!");
            gb.display.cursorX = 6;
            gb.display.cursorY += 8;
            gb.display.print(x);
            gb.display.print("x");
            gb.display.print(y);
            gb.display.print(" ");
            gb.display.print(ruleSet[rule]);
            gb.display.cursorX = 6;
            gb.display.cursorY += 8;
            gb.display.print("Time: ");
            gb.display.print(time / 20);
            gb.display.print(".");
            gb.display.print(time % 20 * 5);
            gb.display.print(" s");
            gb.display.cursorX = 6;
            gb.display.cursorY += 8;
            gb.display.print("Toggles: ");
            gb.display.print(toggles);
            gb.display.cursorX = 6;
            gb.display.cursorY += 8;
            gb.display.print("Press A to exit.");
            if(gb.buttons.pressed(BTN_A)) break;
        }
    }
    gb.sound.playOK();
    return true;
}

// Draw board
void draw(char* board, int x, int y, int hOff, int vOff) {
    // Draw borders
    gb.display.drawFastVLine(hOff - 1, vOff - 1, y * 4 + 2);
    gb.display.drawFastHLine(hOff - 1, vOff - 1, x * 4 + 2);
    gb.display.drawFastVLine(hOff + x * 4, vOff, y * 4 + 1);
    gb.display.drawFastHLine(hOff, vOff + y * 4, x * 4 + 0);
    // Draw board
    for(int i = 0; i < y; i++) {
        for(int k = 0; k < x; k++) {
            gb.display.drawBitmap(hOff + k * 4, vOff + i * 4, field[board[i * x + k]]);
        }
    }
}

// The main game.
void game(int x, int y, int rule) {
    int px = 0, py = 0;
    int hOff = (XMAX - x) * 2;
    int vOff = (YMAX - y) * 2;
    char* board = newBoard(x, y, rule);
    int startT = gb.frameCount;
    int toggles = 0;
    if(won(board, x, y, rule, startT, toggles)) {
        free(board);
        return;
    }
    if(x >= 20 && y > 10) gb.battery.show = false;
    while(true) {
        if(gb.update()) {
            draw(board, x, y, hOff, vOff);
            // Draw indicator for position
            gb.display.drawPixel(hOff + px * 4, vOff + py * 4);
            gb.display.drawPixel(hOff + px * 4 + 3, vOff + py * 4);
            gb.display.drawPixel(hOff + px * 4, vOff + py * 4 + 3);
            gb.display.drawPixel(hOff + px * 4 + 3, vOff + py * 4 + 3);
            // Handle buttons
            if(gb.buttons.pressed(BTN_B)) py = 0;
            else if(gb.buttons.pressed(BTN_A)) {
                toggle(board, x, y, px, py, rule);
                toggles++;
                gb.sound.playTick();
                // Check for win and exit (if won).
                if(won(board, x, y, rule, startT, toggles)) break;
            }
            // Movement
            else if(gb.buttons.pressed(BTN_UP)) py = py <= 0 ? y - 1 : py - 1;
            else if(gb.buttons.pressed(BTN_DOWN)) py = py >= y - 1 ? 0 : py + 1;
            else if(gb.buttons.pressed(BTN_LEFT)) px = px <= 0 ? x - 1 : px - 1;
            else if(gb.buttons.pressed(BTN_RIGHT)) px = px >= x - 1 ? 0 : px + 1;
            // Exit
            else if(gb.buttons.pressed(BTN_C)) {
                gb.sound.playCancel();
                break;
            }
        }
    }
    free(board);
    gb.battery.show = true;
}

void setup(){
    gb.begin();
    gb.pickRandomSeed();
    introBoard();
    gb.titleScreen(F("Lights Out AD by"), screenL);
}

void loop() {
    int x = 5, y = 5, pos = 0, rule = 0;
    bool sound = gb.sound.getVolume() > 0 ? 1 : 0;
    while(1) {
        /* Main menu
         * 0. Play
         * 1. Set width
         * 2. Set height
         * 3. Rules
         * 4. Sound
         */
        if(gb.update()) {
            // Draw Menu
            gb.display.drawCircle(6, 7 + pos * 8, 2);
            gb.display.cursorX = 12;
            gb.display.cursorY = 5;
            gb.display.print("Play");
            gb.display.cursorX = 12;
            gb.display.cursorY += 8;
            gb.display.print("Width:  ");
            gb.display.print(x);
            gb.display.cursorX = 12;
            gb.display.cursorY += 8;
            gb.display.print("Height: ");
            gb.display.print(y);
            gb.display.cursorX = 12;
            gb.display.cursorY += 8;
            gb.display.print("Rules:  ");
            gb.display.print(ruleSet[rule]);
            gb.display.cursorX = 12;
            gb.display.cursorY += 8;
            gb.display.print("Sound:  ");
            gb.display.print(toggleS[sound]);
            // Handle buttons
            if(gb.buttons.pressed(BTN_A)) {
                gb.sound.playTick();
                switch(pos) {
                    case 0:
                        game(x, y, rule);
                        break;
                    case 4:
                        (sound ^= true) ? gb.sound.setVolume(VOLUME_GLOBAL_MAX) : gb.sound.setVolume(0);
                        break;
                }
            } else if(gb.buttons.repeat(BTN_LEFT, 4)) {
                switch(pos) {
                    case 1:
                        x = x == 1 ? XMAX : x - 1;
                        gb.sound.playTick();
                        break;
                    case 2:
                        y = y == 1 ? YMAX : y - 1;
                        gb.sound.playTick();
                        break;
                    case 3:
                        rule = rule == 0 ? RULES - 1 : rule - 1;
                        gb.sound.playTick();
                        break;
                }
            } else if(gb.buttons.repeat(BTN_RIGHT, 4)) {
                switch(pos) {
                    case 1:
                        x = max(1, (x + 1) % (XMAX + 1));
                        gb.sound.playTick();
                        break;
                    case 2:
                        y = max(1, (y + 1) % (YMAX + 1));
                        gb.sound.playTick();
                        break;
                    case 3:
                        rule = (rule + 1) % RULES;
                        gb.sound.playTick();
                        break;
                }
            } else if(gb.buttons.pressed(BTN_UP)) {
                gb.sound.playTick();
                // Modulo is somewhat odd for negative numbers
                // maybe it's just me
                pos = pos == 0 ? 4 : pos - 1;
            } else if(gb.buttons.pressed(BTN_DOWN)) {
                gb.sound.playTick();
                pos = (pos + 1) % 5;
            } else if(gb.buttons.pressed(BTN_C)) {
                gb.changeGame();
            }
        }
    }
}







