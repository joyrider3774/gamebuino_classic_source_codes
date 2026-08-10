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

void play()
{
    next_turn();
    
    game_running = true;
    while (game_running)
    {
        if (gb.update())
        {
            if (senet_has_won(&game_state, player2) || 
                senet_has_won(&game_state, player1))
            {
                game_started = false;
                display_winner();
                return;
            }
            
            if (move_timer < 255) move_timer++;
            
            if (gb.buttons.pressed(BTN_C))
            {
                gb.sound.playCancel();
                return;
            }
            
            draw_player_name();
            draw_board();
            
            draw_game_state();
            
            // select play method to corresponding game mode
            switch (game_mode)
            {
                case GAMEMODE_SINGLE:
                    play_single_player();
                    break;
                case GAMEMODE_MULTI:
                    play_multi_player();
                    break;
                case GAMEMODE_MULTI_I2C:
                    play_multi_player_i2c();
                    break;
            }
        }
    }
}

void play_single_player()
{
    if (game_state.current_player == player2)
    {
        // let CPU move
        cpu_move();
    }
    else
    {
        // the human player has to move
        local_player_move();
    }
}

void play_multi_player()
{
    // pause game to let player exchange the Gamebuino
    if (round_started && game_state.current_player != previous_player)
    {
        previous_player = game_state.current_player;
        bool btn_a_pressed = false;
        while (true)
        {
            if (gb.update())
            {
                if (btn_a_pressed)
                {
                    break;
                }
                
                if (gb.buttons.pressed(BTN_A))
                {
                    gb.sound.playOK();
                    btn_a_pressed = true;
                }
                
                gb.display.print(F("\nNext turn for:\n"));
                gb.display.cursorX = 1 * gb.display.fontWidth;
                gb.display.fontSize = 2;
                if (game_state.current_player == player1)
                {
                    gb.display.println(name_player1);
                }
                else
                {
                    gb.display.println(name_player2);
                }
                gb.display.fontSize = 1;
                
                gb.display.print(F("\n\nPress \x15 to continue."));
            }
        }
    }
    
    local_player_move();
}

void play_multi_player_i2c()
{    
    if (is_master)
    {
        if (round_started)
        {
            master_set_game_state();
            master_send_next_turn();
        }
        
        if (!master_check_timeout())
        {
            connected = false;
            game_running = false;
        }
        
        
        if (game_state.current_player == player1)
        {
            master_set_selected_square();
            
            local_player_move();
        }
        else
        {
            round_started = false;
            master_get_selected_square();
            
            if (master_get_turn_finished())
            {
                master_get_game_state();
                next_turn();
            }
        }
    }
    else
    {
        if (!slave_check_timeout())
        {
            gb.popup(F("Connection lost"), 10);
            
            connected = false;
            game_running = false;
            return;
        }
        
        if (game_state.current_player == player2)
        {
            local_player_move();
        }
    }
}


void cpu_move()
{
    if (round_started)
    {
        if (senet_get_best_move(&game_state))
        {
            selected_square = game_state.current_move.start;
        }
        else
        {
            selected_square = SENET_MAX_SQUARE + 1;
        }
    }
    
    // cpu player moves
    round_started = false;
    
    if (move_timer >= 10 && !game_state.turn_finished)
    {
        // Make the CPU's move
        if (senet_get_best_move(&game_state)) // TODO: this second call is unnecessary
        {
            senet_do_current_move(&game_state);
        }
        else
        {
            gb.popup(F("No move. CPU skips."), 15);
        }
        selected_square = SENET_MAX_SQUARE + 1;
        move_timer = 0;
    }
    else if (move_timer >= 30)
    {
        next_turn();
    }
}


uint8_t btnDown_held = 0;

void local_player_move()
{
    uint8_t row, column;
    
    // check if there is any move to do
    if (round_started)
    {
        selected_square = SENET_MIN_SQUARE;
        while (senet_get_piece(&game_state, selected_square) != game_state.current_player)
        {
            selected_square++;
        }
        
        if (!senet_search_possible_move(&game_state, &(game_state.current_move)))
        {
            gb.popup(F("No move for you."), 20);
            move_timer = 0;
        }
        else if (game_state.current_move.moves < 0)
        {
            gb.popup(F("Moving backwards!"), 40);
        }
    }
    
    
    // navigation on board
    square_to_row_col(selected_square, &row, &column);
    
    if (gb.buttons.repeat(BTN_RIGHT, 3))
    {
        gb.sound.playTick();
        if (column < 10) column++;
        else column = 0;
    }
    if (gb.buttons.repeat(BTN_LEFT, 3))
    {
        gb.sound.playTick();
        if (column > 0) column--;
        else column = 9;
    }
    
    if ((btnDown_held >= 5) && gb.buttons.repeat(BTN_DOWN, 10))
    {
        gb.sound.playTick();
        game_state.current_move.start = selected_square + 1;
        if (!senet_search_next_possible_move(&game_state, &(game_state.current_move)))
        {
            game_state.current_move.start = SENET_MIN_SQUARE;
            if (!senet_search_next_possible_move(&game_state, &(game_state.current_move)))
            {
                game_state.current_move.start = selected_square;
            }
        }
        square_to_row_col(game_state.current_move.start, &row, &column);
    }
    else if (gb.buttons.released(BTN_DOWN) && (btnDown_held < 5))
    {
        gb.sound.playTick();
        if (row < 3) row++;
        else row = 0;
    }
    btnDown_held = gb.buttons.timeHeld(BTN_DOWN);
    
    
    if (gb.buttons.repeat(BTN_UP, 5))
    {
        gb.sound.playTick();
        if (row > 0) row--;
        else row = 2;
    }
    
    selected_square = row_col_to_square(row, column);
    
    
    if (gb.buttons.pressed(BTN_A))
    {
        if (senet_get_piece(&game_state, selected_square) != game_state.current_player)
        {
            gb.sound.playCancel();
            gb.popup(F("Can't move piece."), 20);
        }
        else
        {
            game_state.current_move.start = selected_square;
            if (senet_is_valid_move(&game_state, &(game_state.current_move)))
            {
                gb.sound.playOK();
                senet_do_current_move(&game_state);
                move_timer = 0;
            }
            else
            {
                gb.sound.playCancel();
                gb.popup(F("Invalid move!"), 20);
            }
        }
    }
    
    
    round_started = false;
    if (game_mode != GAMEMODE_MULTI_I2C || is_master)
    {
        if (game_state.turn_finished && move_timer >= 5)
        {
            next_turn();
        }
    }
}

void remote_player_move()
{
    
}

