/*
 * SenetGameAI.cpp
 *
 *  Created on: 24.07.2014
 *      Author: Maximilian Timmerkamp
 *
 * 
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

#include "SenetGameAI.h"


int senet_state_value(senet_state_t *state, senet_player_t player)
{
    int result = 0;
    senet_piece_t last_piece = SENET_PLAYER_NONE;
    
    uint8_t pieces_in_a_row = 0;
    
    for (senet_square_t sq = SENET_MIN_SQUARE; sq <= SENET_MAX_SQUARE; sq++)
    {
        if ((senet_get_piece(state, sq) == last_piece) && 
            (last_piece != SENET_PLAYER_NONE) &&
            (!(senet_is_past_corner_square(sq))))
        {
            pieces_in_a_row++;
        }
        else
        {
            pieces_in_a_row = 0;
        }
        last_piece = senet_get_piece(state, sq);
        
        int value = 0;
        switch (sq)
        {
            case 14: case 25: case 27: case 28:
                value = 3*sq;
                break;
            case 29: 
                value = 10*sq;
                break;
            default:
                value = sq;
                break;
        }
        
        if (pieces_in_a_row > 1)
        {
            value *= pieces_in_a_row;
        }
        
        if (last_piece == player)
        {
            result += value;
        }
        else if (last_piece == senet_get_enemy(player))
        {
            result -= value;
        }
    }
    return result;
}

bool senet_get_best_move(senet_state_t *state)
{
    int value, best_value;
    senet_square_t best_start;
    
    if (senet_search_possible_move(state, &(state->current_move)))
    {
        best_value = (int) (-(1 << 15));
        
        do
        {
            senet_do_current_move(state);
            value = (senet_state_value(state, state->current_player) - 
                     senet_state_value(state, senet_get_enemy(state->current_player)));
            
            if (value > best_value)
            {
                best_value = value;
                best_start = state->current_move.start;
            }
            
            senet_undo_move(state, &(state->current_move));
            state->current_move.start++;
        }
        while (senet_search_next_possible_move(state, &(state->current_move)));
        
        state->current_move.start = best_start;
        return true;
    }
    else
    {
        return false;
    }
}
