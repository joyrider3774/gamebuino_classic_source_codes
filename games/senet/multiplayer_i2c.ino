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

/***
 * Communication over I2C:
 * 
 * I want the game state to be synchronized and it would be nice if even the
 * selected square is highlighted on the other GB. So there is much to 
 * transfer between the Gamebuinos.
 * 
 * To make things easy, I assume that both players have a valid implementation
 * of Senet running. But that also means that there is no protection from 
 * cheating.
 * 
 * - i2c master is always player1
 * 
 * At setup:
 * - 10B name of master
 * - 10B name of slave (to master)
 * - 1B game state:
 *   - 1B pieces_per_player
 * 
 * 
 * At begin of every new turn:
 *  - 20B game state:
 *    - 2B turn_count
 *    - 8B board
 *    - 1B current_player
 *    - 4B current_move:
 *      - 2B start & target
 *      - 1B moves
 *      - 1B player
 *    - 1B turn_finished
 *    - 3B pieces_off_board
 *    - 1B pieces_per_player
 * 
 * While moving:
 *  - 1B selected_square
 */


#define SLAVE_ADDRESS 2

#define CMD_IGNORE 0x00

#define CMD_GET_ROLE 0x80

#define CMD_SETUP 0x31
#define CMD_SET_PLAYER1_NAME 0x32
#define CMD_GET_PLAYER1_NAME 0x33
#define CMD_SET_PLAYER2_NAME 0x34
#define CMD_GET_PLAYER2_NAME 0x35
#define CMD_SET_PLAYER1_ID 0x38

#define CMD_SET_GAME_STATE 0x10
#define CMD_GET_GAME_STATE 0x11
#define CMD_GET_TURN_FINISHED 0x18
#define CMD_NEXT_TURN 0x1A

#define CMD_SET_SELECTED_SQUARE 0x24
#define CMD_GET_SELECTED_SQUARE 0x28

#define ROLE_MASTER 0x80
#define ROLE_SLAVE 0x40

volatile uint8_t command;
volatile uint8_t slave_timer;

void setup_slave()
{
    Wire.begin(SLAVE_ADDRESS);
    Wire.onReceive(slave_receive_event);
    Wire.onRequest(slave_request_event);
    
    command = CMD_IGNORE;
}

void slave_request_event()
{
    //if (is_master) return;
    slave_timer = 0;
    
    switch (command)
    {
        case CMD_GET_GAME_STATE:
            Wire.write((byte*) &game_state, GAME_STATE_LENGTH);
            break;
        case CMD_GET_SELECTED_SQUARE:
            Wire.write(selected_square);
            break;
        case CMD_GET_PLAYER2_NAME:
            Wire.write((const byte*) name_player2, USERNAME_LENGTH);
            break;
        case CMD_GET_PLAYER1_NAME:
            Wire.write((const byte*) name_player1, USERNAME_LENGTH);
            break;
        case CMD_GET_ROLE:
            if (is_master) Wire.write(ROLE_MASTER);
            else Wire.write(ROLE_SLAVE);
            break;
        case CMD_GET_TURN_FINISHED:
            Wire.write(game_state.turn_finished);
            break;
        default:
            gb.popup(F("Communication Error"), 20);
            //connected = false;
            break;
    }
    
    //command = CMD_IGNORE;
}

void slave_receive_event(int num_bytes)
{
    //if (is_master) return;
    slave_timer = 0;
    
    uint8_t pieces;
    command = Wire.read();
    
    switch (command)
    {
        case CMD_SET_GAME_STATE:
            Wire.readBytes((char*) &game_state, GAME_STATE_LENGTH);
            break;
        case CMD_SET_SELECTED_SQUARE:
            selected_square = Wire.read();
            break;
        case CMD_SETUP:
            pieces = Wire.read();
            senet_init_state(&game_state, pieces);
            
            first_move = true;
            game_started = true;
            connected = true;
            break;
        case CMD_SET_PLAYER1_NAME:
            Wire.readBytes((char*) &(name_player1[0]), USERNAME_LENGTH);
            break;
        case CMD_SET_PLAYER2_NAME:
            Wire.readBytes((char*) &(name_player2[0]), USERNAME_LENGTH);
            break;
        case CMD_SET_PLAYER1_ID:
            player1 = Wire.read();
            player2 = senet_get_enemy(player1);
            break;
        case CMD_NEXT_TURN:
            first_move = false;
            round_started = true;
            move_timer = 0;
            display_moves_timer = 0;
            break;
    }
    
    //command = CMD_IGNORE;
}

#define SLAVE_TIMEOUT 5

bool slave_check_timeout()
{
    slave_timer++;
    while (1 < slave_timer && slave_timer < SLAVE_TIMEOUT)
    {
        delay(1);
        slave_timer++;
    }
    
    return slave_timer <= 1; // Only check every second call
}


void setup_master()
{
    Wire.begin();
}

void master_wait_for_connection()
{
    while (!connected && !gb.buttons.pressed(BTN_C))
    {
        if (gb.update())
        {
            gb.display.cursorX = 0;
            gb.display.cursorY = 5;
            
            gb.display.print(F("Waiting for connect.."));
        }
        
        Wire.beginTransmission(SLAVE_ADDRESS);
        Wire.write(CMD_GET_ROLE);
        Wire.endTransmission();
        if (Wire.requestFrom(SLAVE_ADDRESS, 1))
        {
            switch (Wire.read())
            {
                case ROLE_MASTER:
                    connected = false;
                    gb.popup(F("1 master max!"), 20);
                    return;
                case ROLE_SLAVE:
                    // Everything is OK other device is slave
                    connected = true;
                    break;
                default:
                    gb.popup(F("Comm. error"), 20);
                    break;
            }
        }
    }
}

void master_init_game()
{
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(CMD_SETUP);
    Wire.write(game_state.pieces_per_player);
    Wire.endTransmission();
    
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(CMD_SET_PLAYER1_ID);
    Wire.write(player1);
    Wire.endTransmission();
    
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(CMD_SET_PLAYER1_NAME);
    Wire.write((const byte*)(name_player1), USERNAME_LENGTH);
    Wire.endTransmission();
    
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(CMD_GET_PLAYER2_NAME);
    Wire.endTransmission();
    
    if (Wire.requestFrom(SLAVE_ADDRESS, USERNAME_LENGTH))
    {
        Wire.readBytes((char*)(name_player2), USERNAME_LENGTH);
    }
    else
    {
        connected = false;
    }
}

void master_set_game_state()
{
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(CMD_SET_GAME_STATE);
    Wire.write((byte*) &game_state, GAME_STATE_LENGTH);
    Wire.endTransmission();
}

void master_get_game_state()
{
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(CMD_GET_GAME_STATE);
    Wire.endTransmission();
    
    if (Wire.requestFrom(SLAVE_ADDRESS, GAME_STATE_LENGTH))
    {
        Wire.readBytes((char*) &game_state, GAME_STATE_LENGTH);
    }
}

void master_set_selected_square()
{
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(CMD_SET_SELECTED_SQUARE);
    Wire.write(selected_square);
    Wire.endTransmission();
}

void master_get_selected_square()
{
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(CMD_GET_SELECTED_SQUARE);
    Wire.endTransmission();
    
    if (Wire.requestFrom(SLAVE_ADDRESS, 1))
    {
        selected_square = Wire.read();
    }
}

bool master_get_turn_finished()
{
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(CMD_GET_TURN_FINISHED);
    Wire.endTransmission();
    
    if (Wire.requestFrom(SLAVE_ADDRESS, 1))
    {
        return Wire.read();
    }
    return false;
}

void master_send_next_turn()
{
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(CMD_NEXT_TURN);
    Wire.endTransmission();
}


bool master_check_timeout()
{
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(CMD_GET_ROLE);
    Wire.endTransmission();
    
    if (Wire.requestFrom(SLAVE_ADDRESS, 1))
    {
        Wire.read();
        return true;
    }
    else
    {
        return false;
    }
}
