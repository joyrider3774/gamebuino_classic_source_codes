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


const char PROGMEM menu_str_play[] = "Play / Resume";
const char PROGMEM menu_str_restart[] = "Restart";
const char PROGMEM menu_str_help[] = "Help";
const char PROGMEM menu_str_controls[] = "Controls";
//const char PROGMEM menu_str_load[] = "Load state";
//const char PROGMEM menu_str_save[] = "Save state";
const char PROGMEM menu_str_exit[] = "Main Menu";

const byte MAIN_MENU_LENGTH = 5;
const char* const main_menu[MAIN_MENU_LENGTH] PROGMEM = {
    menu_str_play,
    menu_str_restart,
    menu_str_help,
    menu_str_controls,
    //menu_str_save,
    //menu_str_load,
    menu_str_exit
};

void display_main_menu()
{
    switch (gb.menu(main_menu, MAIN_MENU_LENGTH))
    {
        case 0: // play / resume
            if (!game_started)
            {
                game_setup();
            }
            
            if (game_started)
            {
                play();
            }
            break;
        case 1: // restart
            if (game_setup())
            {
                //game_started = true;
                play();
            }
            break;
        case 2: // help
            display_help();
            break;
        case 3: // controls
            display_controls();
            break;
        /*case 3: // save state
            
            break;
        case 4: // load state
            
            break;*/
        case 4: // exit / display gamebuino main menu
        default:
            gb.titleScreen(F("Senet"), senet_logo);
            break;
    }
}


const char PROGMEM HELP_PAGE0[] = (
    " -=- SENET INTRO -=-\n"
    "Senet is an old game played by everyone in ancient Egypt. "
    "Using proper boards and pieces or just the street and some "
    "stone and wood."
);

const char PROGMEM HELP_PAGE1[] = (
    " -=- SENET RULES -=-\n"
    "START: Each player gets 3-7 pieces placed alternating "
    "starting at square 1.\n"
    "MOVE: 4 two-sided paddles are thrown to determine the player's "
);
const char PROGMEM HELP_PAGE2[] = (
    "moves for one piece.\n"
    "Moving direction:\n"
    "|1|>|>|>|>|>|>|>|>|v|"
    "|v|<|<|<|<|O|<|<|<|<|"
    "|>|>|>|>|>|V|~|>|>|I|"
    "~: Trap; O: House of Rebirth; I: sq30 Goal"
);

const char PROGMEM HELP_PAGE3[] = (
//   #####################  WIDTH for 3x5 wide characters
    " white +      +throw\n"
    " sides +moves +again\n"
    "   1   |   1  | Yes\n"
    "   2   |   2  | No\n"
    "   3   |   3  | No\n"
    "   4   |   4  | Yes\n"
    "   0   |   6  | Yes\n"
    "(4x black => 6 moves)"
);

const char PROGMEM HELP_PAGE4[] = (
    "You move again if throwing a 1, 4 or 0.\n"
    "ATTACK: Landing on an opponents' piece is an attack,and you exchange "
    "places; you may not land on your own pieces."
);

const char PROGMEM HELP_PAGE5[] = (
    "RESTRICTIONS:\n"
    "-First move must move the piece on highest square.\n"
    "-Safe squares: 15, 26, 28 and 29 cannot be attacked.\n"
);
const char PROGMEM HELP_PAGE6[] = (
    "-Defence: Two or more opponent pieces in a row cannot be attacked.\n"
    "-Blockade: Three or more opponent pieces in a row cannot be passed; -> "
);

const char PROGMEM HELP_PAGE7[] = (
    " however blockades do not  turn around corners.\n"
    "-Trap: Land on square 20 means moving back to the first empty square "
    "before sq.16.\n"
);
const char PROGMEM HELP_PAGE8[] = (
    "-Exit: You may not move past 30. A piece on 30 can be removed at "
    "START of your turn if all your pieces are out of first row.\n"
    "-No Move: If you cannot move foreward, "
);
const char PROGMEM HELP_PAGE9[] = (
    " you must move backward (according to the same rules). If no "
    "move is possible, your turn ends.\n"
    "\nWIN: You win by removing all your pieces."
);

const uint8_t HELP_PAGE_COUNT = 10;
const char* const HELP_PAGES[HELP_PAGE_COUNT] PROGMEM = {
    HELP_PAGE0,
    HELP_PAGE1,
    HELP_PAGE2,
    HELP_PAGE3,
    HELP_PAGE4,
    HELP_PAGE5,
    HELP_PAGE6,
    HELP_PAGE7,
    HELP_PAGE8,
    HELP_PAGE9
};


void display_help()
{
    gb.battery.show = false;
    uint8_t page = 0;
    
    for (;;)
    {
        if (gb.update())
        {
            if (gb.buttons.pressed(BTN_B) || gb.buttons.pressed(BTN_C))
            {
                gb.sound.playCancel();
                gb.battery.show = true;
                return;
            }
            
            if (gb.buttons.pressed(BTN_LEFT))
            {
                gb.sound.playTick();
                if (page > 0) page--;
                else page = HELP_PAGE_COUNT - 1;
            }
            else if (gb.buttons.pressed(BTN_RIGHT) || gb.buttons.pressed(BTN_A))
            {
                gb.sound.playTick();
                page = (page + 1) % HELP_PAGE_COUNT;
            }
            
            gb.display.cursorX = 0;
            gb.display.cursorY = 0;
            gb.display.textWrap = true;
            gb.display.print((const __FlashStringHelper*)pgm_read_word(HELP_PAGES + page));
        }
    }
}


const char PROGMEM CONTROLS_STR[] = (    
//   #####################  WIDTH for 3x5 wide characters
    "  -=- CONTROLS -=-\n"
    "\x15: move piece\n"
    "\x18\x19\x1A\x1B: move selection\n"
    " hold \x19 to get next possible move\n"
    "\x17: back to menu\n"
);

void display_controls()
{
    gb.battery.show = false;
    
    for (;;)
    {
        if (gb.update())
        {
            if (gb.buttons.pressed(BTN_A) || gb.buttons.pressed(BTN_B) || 
                gb.buttons.pressed(BTN_C))
            {
                gb.sound.playOK();
                gb.battery.show = true;
                return;
            }
            
            gb.display.cursorX = 0;
            gb.display.cursorY = 0;gb.display.textWrap = true;
            gb.display.print((const __FlashStringHelper*)CONTROLS_STR);
        }
    }
}
