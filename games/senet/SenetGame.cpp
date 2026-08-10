/*
 * SenetGame.cpp
 *
 *  Created on: 15.05.2014
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

#include "SenetGame.h"
#include <Arduino.h>

boolean senet_is_past_corner_square(senet_square_t square)
{
    return (square == 10) || (square == 20);
}

boolean senet_is_protected_square(senet_square_t square)
{
    return (square == 14) || (square == 25) || (square == 27) || (square == 28);
}


void senet_set_piece(senet_state_t* state, senet_square_t square, senet_piece_t piece)
{
    (state->board)[square / 4] = ((piece & 0x3) << 2*(square % 4)) |
            (state->board[square / 4] & ~(0x3 << 2*(square % 4)));
}

senet_piece_t senet_get_piece(senet_state_t* state, senet_square_t square)
{
    return ((state->board)[square / 4] >> 2*(square % 4)) & 0x3;
}


senet_player_t senet_get_enemy(senet_player_t player)
{
    switch (player)
    {
    case SENET_PLAYER1: return SENET_PLAYER2;
    case SENET_PLAYER2: return SENET_PLAYER1;
    default: return SENET_PLAYER_NONE;
    }
}


boolean senet_is_blocked(senet_state_t* state, senet_move_t* move)
{
    senet_square_t i, start, end;
    uint8_t count = 0;

    if (move->moves > 0)
    {
        start = max(SENET_MIN_SQUARE, move->start + 1);
        end = min(SENET_MAX_SQUARE, move->start + move->moves);
    }
    else
    {
        start = max(SENET_MIN_SQUARE, move->start - 1);
        end = min(SENET_MAX_SQUARE, move->start + move->moves);
    }

    for (i = start; i <= end; i++)
    {
        // Blockades may not turn around corners
        if (senet_is_past_corner_square(i)) count = 0;

        if (senet_get_piece(state, i) == senet_get_enemy(state->current_player))
        {
            count++;
            if (count >= 3) return true;
        }
        else
        {
            count = 0;
        }
    }

    return false;
}

boolean senet_can_attack(senet_state_t* state, senet_square_t square, senet_player_t player)
{
    senet_square_t i;
    uint8_t count = 0;

    if (senet_get_piece(state, square) == SENET_PLAYER_NONE) return true;
    if (senet_get_piece(state, square) == player) return false;
    if (senet_is_protected_square(square)) return false;
    
    const senet_square_t start = max(SENET_MIN_SQUARE, square - 1);
    const senet_square_t end = min(SENET_MAX_SQUARE, square + 1);

    for (i = start; i <= end; i++)
    {
        if (senet_get_piece(state, i) == senet_get_enemy(state->current_player))
        {
            count++;

            if (count >= 2) return false;
        }
        else
        {
            count = 0;
        }
    }

    return true;
}

boolean senet_is_valid_move(senet_state_t* state, senet_move_t* move)
{
    // first move in this game
    if (state->turn_count == 0)
    {
        return move->start == SENET_MIN_SQUARE + 2*state->pieces_per_player - 1;
    }
    else
    {
        senet_square_t target = move->start + move->moves;
        return ((target <= SENET_MAX_SQUARE) && (target >= SENET_MIN_SQUARE) &&
                (senet_get_piece(state, target) != move->player) &&
                senet_can_attack(state, target, move->player) &&
                !senet_is_blocked(state, move));
    }
}


boolean senet_do_current_move(senet_state_t* state)
{
    return senet_do_move(state, &(state->current_move));
}

boolean senet_do_move(senet_state_t* state, senet_move_t* move)
{
    if (senet_is_valid_move(state, move))
    {
        move->target = move->start + move->moves;

        senet_piece_t target_piece = senet_get_piece(state, move->target);
        senet_set_piece(state, move->target, move->player);
        senet_set_piece(state, move->start, target_piece);

        state->turn_finished = true;

        if (move->target == SENET_SQUARE_WATER)
        {
            senet_handle_house_of_water(state, move);
        }
    }
    else
    {
        state->turn_finished = false;
    }

    return state->turn_finished;
}

void senet_undo_move(senet_state_t* state, senet_move_t* move)
{
    // Just swap stones again to undo move.
    senet_square_t target_piece = senet_get_piece(state, move->target);
    senet_set_piece(state, move->target, senet_get_piece(state, move->start));
    senet_set_piece(state, move->start, target_piece);

    // No checking for special fields.

    state->turn_finished = false;
}

void senet_handle_house_of_water(senet_state_t* state, senet_move_t* move)
{
    senet_square_t square;
    for (square = SENET_SQUARE_REPEATING_LIFE; square >= SENET_MIN_SQUARE; square--)
    {
        if (senet_get_piece(state, square) == SENET_PLAYER_NONE)
        {
            senet_set_piece(state, square, senet_get_piece(state, SENET_SQUARE_WATER));
            senet_set_piece(state, SENET_SQUARE_WATER, SENET_PLAYER_NONE);
            move->target = square;
            return;
        }
    }
}


boolean senet_remove_piece_from_square_30(senet_state_t* state)
{
    if (!state->turn_finished &&
            (senet_get_piece(state, SENET_SQUARE_I) == state->current_player))
    {
        // Check if there is no piece left in first row
        for (senet_square_t square = SENET_MIN_SQUARE; square < 10; square++)
        {
            if (senet_get_piece(state, square) == state->current_player)
            {
                return false;
            }
        }
        
        senet_set_piece(state, SENET_SQUARE_I, SENET_PLAYER_NONE);
        state->pieces_off_board[state->current_player]++;
        return true;
    }

    return false;
}

boolean senet_has_won(senet_state_t* state, senet_player_t player)
{
    return state->pieces_off_board[player] == state->pieces_per_player;
}


boolean senet_search_possible_move(senet_state_t* state, senet_move_t* move)
{
    senet_square_t old_start = move->start;

    move->start = SENET_MIN_SQUARE;
    if (senet_search_next_possible_move(state, move)) return true;

    move->moves = -move->moves;
    if (senet_search_next_possible_move(state, move)) return true;

    // No possible move found
    move->start = old_start;
    move->moves = -move->moves;
    state->turn_finished = true;

    return false;
}

boolean senet_search_next_possible_move(senet_state_t* state, senet_move_t* move)
{
    senet_square_t old_start = move->start;
    senet_square_t square;

    for (square = move->start; square <= SENET_MAX_SQUARE; square++)
    {
        if (senet_get_piece(state, square) == move->player)
        {
            move->start = square;
            if (senet_is_valid_move(state, move))
            {
                return true;
            }
        }
    }

    move->start = old_start;

    return false;
}


uint8_t senet_throw_paddles()
{
    uint8_t count = 0;
    uint8_t i;

    for (i = 0; i < 4; i++)
    {
        if ((random() & 0x1) == 1)
        {
            count++;
        }
    }

    if (count == 0) count = 6;

    return count;
}


void senet_begin_turn(senet_state_t* state)
{
    if (state->turn_finished)
    {
        state->current_move.moves = senet_throw_paddles();
        state->current_move.start = 0;
        state->current_move.player = state->current_player;

        state->turn_finished = false;
    }
}

void senet_end_turn(senet_state_t* state)
{
    // Do not end turn if no piece has moved
    if (!state->turn_finished) return;

    switch (state->current_move.moves)
    {
    case 6: case 1: case 4: break; // state->current_player is not changed
    default:
        state->current_player = senet_get_enemy(state->current_player);
    }

    state->turn_count++;
}

void senet_init_state(senet_state_t* state, uint8_t pieces)
{
    uint8_t i;
    for (i = SENET_MIN_SQUARE; i < SENET_MIN_SQUARE + 2*pieces;)
    {
        senet_set_piece(state, i++, SENET_PLAYER1);
        senet_set_piece(state, i++, SENET_PLAYER2);
    }

    for (i = 2*pieces; i <= SENET_MAX_SQUARE; i++)
    {
        senet_set_piece(state, i, SENET_PLAYER_NONE);
    }

    state->turn_count = 0;
    state->turn_finished = true;
    state->current_player = SENET_PLAYER2;

    state->pieces_per_player = pieces;
    state->pieces_off_board[SENET_PLAYER1] = 0;
    state->pieces_off_board[SENET_PLAYER2] = 0;
}
