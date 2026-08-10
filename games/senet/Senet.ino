/*
 * Copyright 2014 Maximilian Timmerkamp
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <SPI.h>
#include <Wire.h>
#include <Gamebuino.h>

#include "SenetGame.h"
#include "SenetGameAI.h"


#define I2C_TIMEOUT 50


// "import" fonts
extern const byte font3x5[];
//extern const byte font5x7[];

// get tiles and sprites from "tiles.ino"
extern const byte PROGMEM tile_repeating_life[];
extern const byte PROGMEM tile_square_v[];
extern const byte PROGMEM tile_square_water1[];
extern const byte PROGMEM tile_square_water2[];
extern const byte PROGMEM tile_square_iii[];
extern const byte PROGMEM tile_square_ii[];
extern const byte PROGMEM tile_square_i[];
extern const byte PROGMEM piece_player1[];
extern const byte PROGMEM piece_player2[];
extern const byte* const bitmap_pieces[];
extern const byte PROGMEM senet_logo[];


// Strings for multiplayer selection
const byte STR_GAMEMODES_LENGTH = 11;
//                                             ###################
const char STR_GAMEMODE_SINGLE[] PROGMEM =    "  single   ";
const char STR_GAMEMODE_MULTI[] PROGMEM =     "   multi   ";
const char STR_GAMEMODE_MULTI_I2C[] PROGMEM = "multi (i2c)";

const uint8_t GAMEMODE_SINGLE = 0;     // single player
const uint8_t GAMEMODE_MULTI = 1;      // multi player hot seat
const uint8_t GAMEMODE_MULTI_I2C = 2;  // multi player using I2C

const uint8_t GAMEMODES_LENGTH = 3;
const char* const STR_GAMEMODES[GAMEMODES_LENGTH] PROGMEM = {
    STR_GAMEMODE_SINGLE,
    STR_GAMEMODE_MULTI,
    STR_GAMEMODE_MULTI_I2C,
};


Gamebuino gb = Gamebuino();

uint8_t game_mode = GAMEMODE_SINGLE;
bool is_master = false;
volatile bool connected = false;

volatile bool game_running = false;
volatile bool game_started = false;
senet_state_t game_state;

volatile bool first_move = true;
volatile bool round_started = true;

volatile senet_square_t selected_square;
volatile senet_player_t player1;     // human player in single player mode
volatile senet_player_t player2;     // CPU player in single player mode
senet_player_t previous_player; // player which did the previous turn

volatile uint8_t move_timer = 0;     // Used to make CPU moves slow / better to recognise
volatile uint8_t display_moves_timer = 0; // Used to display throwing the paddles

// buffers for the players' names
char name_player1[USERNAME_LENGTH];
char name_player2[USERNAME_LENGTH];


void setup()
{
    gb.begin();
    gb.display.setFont(font3x5);
    gb.titleScreen(F("Senet"), senet_logo);
    
    gb.pickRandomSeed();
    
    name_player2[0] = 'P';
    name_player2[1] = 'l';
    name_player2[2] = 'a';
    name_player2[3] = 'y';
    name_player2[4] = 'e';
    name_player2[5] = 'r';
    name_player2[6] = '2';
    name_player2[7] = 0;
}

void loop()
{
    display_main_menu();
}


void next_turn()
{
    move_timer = 0;
    display_moves_timer = 0;
    
    if (first_move)
    {
        senet_begin_turn(&game_state);
        first_move = false;
    }
    else if (game_state.turn_finished)
    {
        senet_end_turn(&game_state);
        senet_begin_turn(&game_state);
        
        // automatically remove a piece from square 30 (if possible)
        senet_remove_piece_from_square_30(&game_state);
    }
    
    round_started = true;
}

bool game_setup()
{
    player1 = 1 + random(2); // choose an internal player number
    player2 = senet_get_enemy(player1);
    
    uint8_t pieces = PIECE_COUNT_MIN;
    for (;;)
    {
        if (gb.update())
        {
            if (gb.buttons.pressed(BTN_A))
            {
                switch (game_mode)
                {
                    case GAMEMODE_SINGLE:
                        game_started = setup_single();
                        break;
                    case GAMEMODE_MULTI:
                        game_started = setup_multi();
                        break;
                    case GAMEMODE_MULTI_I2C:
                        game_started = setup_multi_i2c();
                        break;
                    default:
                        game_started = false;
                        break;
                }
                
                if (game_started)
                {
                    senet_init_state(&game_state, pieces);
                    first_move = true;
                    selected_square = 0;
                }
                return game_started;
            }
            if (gb.buttons.pressed(BTN_B) || gb.buttons.pressed(BTN_C))
            {
                gb.sound.playCancel();
                return false;
            }
            
            if (gb.buttons.repeat(BTN_LEFT, 5))
            {
                gb.sound.playTick();
                if (pieces == PIECE_COUNT_MIN) pieces = PIECE_COUNT_MAX;
                else pieces -= 1;
            }
            else if (gb.buttons.repeat(BTN_RIGHT, 5))
            {
                gb.sound.playTick();
                if (pieces == PIECE_COUNT_MAX) pieces = PIECE_COUNT_MIN;
                else pieces += 1;
            }
            
            if (gb.buttons.repeat(BTN_DOWN, 5))
            {
                gb.sound.playTick();
                game_mode = (game_mode + 1) % GAMEMODES_LENGTH;
            }
            else if (gb.buttons.repeat(BTN_UP, 5))
            {
                gb.sound.playTick();
                if (game_mode > 0) game_mode--;
                else game_mode = GAMEMODES_LENGTH - 1;
            }
            
            gb.display.cursorX = (LCDWIDTH - 20*gb.display.fontWidth) / 2;
            gb.display.cursorY = 0;
            gb.display.print(F(" -=- GAME SETUP -=- "));
            
            
            // mode selection
            gb.display.cursorX = 0;
            gb.display.cursorY = 1*gb.display.fontHeight;
            gb.display.print(F("Game Mode:"));
            
            gb.display.cursorX = (LCDWIDTH - (STR_GAMEMODES_LENGTH + 2)*gb.display.fontWidth) / 2;
            gb.display.cursorY += gb.display.fontHeight;
            gb.display.print('\x1e');
            gb.display.print((const __FlashStringHelper*)pgm_read_word(STR_GAMEMODES + game_mode));
            gb.display.print('\x1f');
            
            
            // "Pieces per Player:"
            gb.display.cursorX = 0;
            gb.display.cursorY = 3*gb.display.fontHeight;
            gb.display.print(F("Pieces per Player:"));
            
            gb.display.drawChar(LCDWIDTH - 5*gb.display.fontWidth, 4*gb.display.fontHeight, 0x11, 1); // <
            gb.display.drawChar(LCDWIDTH - 1*gb.display.fontWidth, 4*gb.display.fontHeight, 0x10, 1); // >
            // draw piece count
            gb.display.drawChar(LCDWIDTH - 3*gb.display.fontWidth, 4*gb.display.fontHeight, '0' + (char) pieces, 1);
            
            
            // "Your Pieces:"
            gb.display.cursorX = 0;
            gb.display.cursorY = 5*gb.display.fontHeight;
            gb.display.print(F("Your Pieces:"));
            gb.display.drawBitmap(13*gb.display.fontWidth, gb.display.cursorY, bitmap_pieces[player1 - 1]);
            
            
            // Controls
            gb.display.cursorX = 0;
            gb.display.cursorY = LCDHEIGHT - gb.display.fontHeight;
            gb.display.print(F("\x16:Cancel"));
            
            gb.display.cursorX = LCDWIDTH - 4*gb.display.fontWidth;
            gb.display.print(F("OK:\x15"));
        }
    }
    return true;
}

bool setup_single()
{
    // in single player mode player1 is always the local player
    gb.getDefaultName(name_player1);
    
    // in single player mode player2 is always "CPU"
    // handled in the rendering methods
    
    gb.sound.playOK();
    return true;
}

bool setup_multi()
{
    // player1 is always the local player
    gb.getDefaultName(name_player1);
    
    // do not set player2 name to a default
    gb.keyboard(name_player2, USERNAME_LENGTH);
    
    gb.sound.playOK();
    return true;
}

const char PROGMEM STR_MP_I2C_MASTER[] = "Host (master)";
const char PROGMEM STR_MP_I2C_SLAVE[] = "Join (slave)";
const char PROGMEM STR_MP_I2C_CANCEL[] = "Cancel";

const byte MP_I2C_MENU_LENGTH = 3;
const char* const MP_I2C_MENU[MP_I2C_MENU_LENGTH] PROGMEM = {
    STR_MP_I2C_MASTER,
    STR_MP_I2C_SLAVE,
    STR_MP_I2C_CANCEL,
};

bool setup_multi_i2c()
{    
    gb.sound.playOK();
    
    switch (gb.menu(MP_I2C_MENU, MP_I2C_MENU_LENGTH))
    {
        case 0: // Master
            is_master = true;
            break;
        case 1: // Slave
            is_master = false;
            break;
        default: // Cancel
            gb.sound.playCancel();
            return false;
    }
    
    connected = false;
    if (is_master)
    {
        gb.getDefaultName(name_player1);
        
        setup_slave();
        setup_master();
        
        master_wait_for_connection();
        master_init_game();
    }
    else
    {
        gb.getDefaultName(name_player2);
        
        setup_slave();
        setup_master();
        
        while (!connected && !gb.buttons.pressed(BTN_C))
        {
            delay(1); // wait for connection
        }
        if (!connected)
        {
            gb.popup(F("Timeout: No connect"), 20);
        }
    }
    
    return connected;
}

void display_winner()
{
    while (true)
    {
        if (gb.update())
        {
            if (gb.buttons.pressed(BTN_A) || gb.buttons.pressed(BTN_B) || 
                gb.buttons.pressed(BTN_C))
            {
                gb.sound.playOK();
                return;
            }
            
            gb.display.fontSize = 2;
            gb.display.cursorX = (LCDWIDTH - 2 * 10 * gb.display.fontWidth) / 2;
            gb.display.cursorY = 2;
            gb.display.print(F("GAME OVER!"));
            
            gb.display.cursorY = 2 + 2 * gb.display.fontHeight;
            gb.display.cursorX = (LCDWIDTH - 2 * 7 * gb.display.fontWidth) / 2;
            if (senet_has_won(&game_state, player1))
            {
                gb.display.print(F("YOU WON"));
            }
            else
            {
                gb.display.print(F("CPU WON"));
            }
            
            gb.display.cursorX = (LCDWIDTH - 3 * gb.display.fontWidth) / 2;
            gb.display.cursorY = 2 + 4 * gb.display.fontHeight;
            gb.display.drawChar(gb.display.cursorX, gb.display.cursorY, '0' + (char)game_state.pieces_off_board[player1], 1);
            gb.display.cursorX += gb.display.fontWidth;
            gb.display.drawChar(gb.display.cursorX, gb.display.cursorY, ':', 1);
            gb.display.cursorX += gb.display.fontWidth;
            gb.display.drawChar(gb.display.cursorX, gb.display.cursorY, '0' + (char)game_state.pieces_off_board[player2], 1);
            
            gb.display.fontSize = 1;
            
            
            gb.display.cursorX = LCDWIDTH - 7*gb.display.fontWidth;
            gb.display.cursorY = LCDHEIGHT - gb.display.fontHeight;
            gb.display.print(F("Close:\x15"));
        }
    }
}

void draw_game_state()
{
    const uint8_t x = (LCDWIDTH - 3) / 2;
    const uint8_t y = 2;
    
    //if (display_moves_timer < 255) display_moves_timer++;
    
    if (display_moves_timer < 7)
    {
        display_moves_timer++;
        
        gb.display.setColor(display_moves_timer % 2);
        gb.display.fillRect(x-2, y-2, gb.display.fontWidth+3, gb.display.fontHeight+2);
        gb.display.setColor(1);
    }
    else
    {
        gb.display.drawChar(x, y, '0' + (char) abs(game_state.current_move.moves), 1);
        gb.display.drawRect(x-2, y-2, gb.display.fontWidth+3, gb.display.fontHeight+2);
    }
}

void draw_board()
{
    // Draw the board
    for (uint8_t row = 9; row <= 33; row += 8)
    {
        gb.display.drawFastHLine(1, row, 81);
    }
    for (uint8_t column = 1; column <= 81; column += 8)
    {
        gb.display.drawFastVLine(column, 10, 24);
    }
    
    // draw pieces
    for (senet_square_t square = SENET_MIN_SQUARE; square <= SENET_MAX_SQUARE; square++)
    {
        switch (senet_get_piece(&game_state, square))
        {
            case SENET_PLAYER_NONE:
                switch (square)
                {
                case SENET_SQUARE_REPEATING_LIFE:
                    draw_field(SENET_SQUARE_REPEATING_LIFE, tile_repeating_life);
                    break;
                case SENET_SQUARE_V:
                    draw_field(SENET_SQUARE_V, tile_square_v);
                    break;
                case SENET_SQUARE_WATER:
                    if (gb.frameCount % 20 > 9)
                    { 
                        draw_field(SENET_SQUARE_WATER, tile_square_water1);
                    }
                    else
                    {
                        draw_field(SENET_SQUARE_WATER, tile_square_water2);
                    }
                    break;
                case SENET_SQUARE_III:
                    draw_field(SENET_SQUARE_III, tile_square_iii);
                    break;
                case SENET_SQUARE_II:
                    draw_field(SENET_SQUARE_II, tile_square_ii);
                    break;
                case SENET_SQUARE_I:
                    draw_field(SENET_SQUARE_I, tile_square_i);
                    break;
                }
                break;
            case SENET_PLAYER1:
                draw_field(square, piece_player1);
                break;
            case SENET_PLAYER2:
                draw_field(square, piece_player2);
                break;
        }
        
        // mark special squares
        switch (square)
        {
            case SENET_SQUARE_REPEATING_LIFE:
            case SENET_SQUARE_V:
            case SENET_SQUARE_WATER:
            case SENET_SQUARE_III:
            case SENET_SQUARE_II:
            case SENET_SQUARE_I:
                uint8_t x, y;
                get_square_coordinates(square, &x, &y);
                gb.display.drawPixel(x, y);
                gb.display.drawPixel(x + 6, y);
                gb.display.drawPixel(x, y + 6);
                gb.display.drawPixel(x + 6, y + 6);
                break;
        }
    }
    
    // draw selected square
    if (selected_square <= SENET_MAX_SQUARE)
    {
        uint8_t x, y;
        get_square_coordinates(selected_square, &x, &y);
        x--; // get the border coordinates
        y--;
        
        uint8_t start = gb.frameCount % 10 >= 5; // let it blink
        
        gb.display.setColor(0);
        for (uint8_t i = start; i < 9; i+=2)
        {
            gb.display.drawPixel(x + i, y);
            gb.display.drawPixel(x, y + i);
            gb.display.drawPixel(x + i, y + 8);
            gb.display.drawPixel(x + 8, y + i);
        }
        gb.display.setColor(1);
    }
    
}

void draw_player_name()
{
    uint8_t length;
    length = 0;
    gb.display.cursorX = 0;
    gb.display.cursorY = 1;
    
    while (name_player1[length] != 0)
    {
        gb.display.drawChar(gb.display.cursorX, gb.display.cursorY, name_player1[length], 1);
        
        length++;
        gb.display.cursorX += gb.display.fontWidth;
    }
    if (game_state.current_player == player1)
    {
        gb.display.cursorX = 0;
        gb.display.cursorY += gb.display.fontHeight;
        
        gb.display.drawFastHLine(gb.display.cursorX, gb.display.cursorY, length * gb.display.fontWidth);
    }
    
    if (game_mode == GAMEMODE_SINGLE)
    {
        // display "CPU" as 2nd player name
        
        gb.display.cursorX = LCDWIDTH - 3*gb.display.fontWidth - 3;
        gb.display.cursorY = 1;
        gb.display.print(F("CPU"));
        if (game_state.current_player == player2)
        {
            gb.display.cursorX = LCDWIDTH - 3*gb.display.fontWidth - 3;
            gb.display.cursorY += gb.display.fontHeight;
            
            gb.display.drawFastHLine(gb.display.cursorX, gb.display.cursorY, 3*gb.display.fontWidth);
        }
    }
    else
    {
        length = 0;
        
        while (name_player2[length] != 0)
        {
            length++;
        }
        
        gb.display.cursorX = LCDWIDTH - 3 - length * gb.display.fontWidth;
        gb.display.cursorY = 1;
        gb.display.print(name_player2);
        
        if (game_state.current_player == player2)
        {
            gb.display.cursorX = LCDWIDTH - 3 - length * gb.display.fontWidth;
            gb.display.cursorY += gb.display.fontHeight;
            
            gb.display.drawFastHLine(gb.display.cursorX, gb.display.cursorY, length * gb.display.fontWidth);
        }
    }
    
    gb.display.cursorX = (LCDWIDTH - 3 * gb.display.fontWidth) / 2;
    gb.display.cursorY = 36;
    gb.display.drawChar(gb.display.cursorX, gb.display.cursorY, '0' + (char)game_state.pieces_off_board[player1], 1);
    gb.display.cursorX += gb.display.fontWidth;
    gb.display.drawChar(gb.display.cursorX, gb.display.cursorY, ':', 1);
    gb.display.cursorX += gb.display.fontWidth;
    gb.display.drawChar(gb.display.cursorX, gb.display.cursorY, '0' + (char)game_state.pieces_off_board[player2], 1);
}

void draw_field(byte field, const uint8_t *bitmap)
{
    uint8_t x, y;
    get_square_coordinates(field, &x, &y);
    
    gb.display.drawBitmap(x, y, bitmap);
}

void get_square_coordinates(senet_square_t square, uint8_t *x, uint8_t *y)
{
    square %= SENET_BOARD_SIZE;
    
    if (square < 10) // 1st row
    {
        *y = 10;
        *x = 2 + 8 * square;
    }
    else if (square < 20) // 2nd row
    {
        *y = 18;
        *x = 74 - 8 * (square - 10);
    }
    else // 3rd row
    {
        *y = 26;
        *x = 2 + 8 * (square - 20);
    }
}


senet_square_t row_col_to_square(uint8_t row, uint8_t col)
{
    row %= 3;
    col %= 10;
    
    if (row == 1) col = 9 - col;
    return row * 10 + col;
}

void square_to_row_col(senet_square_t square, uint8_t *row, uint8_t *col)
{
    *row = square / 10;
    *col = square % 10;
    if (*row == 1) *col = 9 - *col;
}

