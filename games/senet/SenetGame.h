/*
 * SenetGame.h
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

#ifndef _SenetGame_H_
#define _SenetGame_H_

#include <Arduino.h>

const uint8_t PIECE_COUNT_MIN = 3;
const uint8_t PIECE_COUNT_MAX = 7;

typedef uint8_t senet_square_t;
const senet_square_t SENET_MIN_SQUARE = 0;
const senet_square_t SENET_MAX_SQUARE = 29;

const senet_square_t SENET_SQUARE_REPEATING_LIFE = 14;
const senet_square_t SENET_SQUARE_V = 25;
const senet_square_t SENET_SQUARE_WATER = 26;
const senet_square_t SENET_SQUARE_III = 27;
const senet_square_t SENET_SQUARE_II = 28;
const senet_square_t SENET_SQUARE_I = 29;

#define SENET_BOARD_SIZE 30
const uint8_t SENET_BOARD_BYTES = SENET_BOARD_SIZE / 4 + 1;
typedef senet_square_t senet_board_t[SENET_BOARD_BYTES];

typedef uint8_t senet_player_t;
const senet_player_t SENET_PLAYER1 = 1;
const senet_player_t SENET_PLAYER2 = 2;
const senet_player_t SENET_PLAYER_NONE = 0;

typedef senet_player_t senet_piece_t;


typedef struct {
    senet_square_t start, target;
    int8_t moves;
    senet_player_t player;
} senet_move_t;


#define GAME_STATE_LENGTH 20
typedef struct {
    uint16_t turn_count;
    senet_board_t board;

    senet_player_t current_player;
    senet_move_t current_move;
    boolean turn_finished;

    uint8_t pieces_per_player;
    uint8_t pieces_off_board[3];
} senet_state_t;


boolean senet_is_past_corner_square(senet_square_t square);
boolean senet_is_protected_square(senet_square_t square);


void senet_set_piece(senet_state_t* state, senet_square_t square, senet_piece_t piece);
senet_piece_t senet_get_piece(senet_state_t* state, senet_square_t square);

senet_player_t senet_get_enemy(senet_player_t player);

boolean senet_is_blocked(senet_state_t* state, senet_move_t* move);
boolean senet_can_attack(senet_state_t* state, senet_square_t square, senet_player_t player);
boolean senet_is_valid_move(senet_state_t* state, senet_move_t* move);

boolean senet_do_current_move(senet_state_t* state);
boolean senet_do_move(senet_state_t* state, senet_move_t* move);
void senet_undo_move(senet_state_t* state, senet_move_t* move);
void senet_handle_house_of_water(senet_state_t* state, senet_move_t* move);

boolean senet_remove_piece_from_square_30(senet_state_t* state);
boolean senet_has_won(senet_state_t* state, senet_player_t player);

boolean senet_search_possible_move(senet_state_t* state, senet_move_t* move);
boolean senet_search_next_possible_move(senet_state_t* state, senet_move_t* move);

uint8_t senet_throw_paddles();

void senet_begin_turn(senet_state_t* state);
void senet_end_turn(senet_state_t* state);
void senet_init_state(senet_state_t* state, uint8_t pieces);


#endif /* _SenetGame_H_ */
