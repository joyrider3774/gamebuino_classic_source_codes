{
  uSenetGame.pas
  Game logic for Senet.
    

  Copyright 2014 Maximilian Timmerkamp

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at
  
  http://www.apache.org/licenses/LICENSE-2.0
  
  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
}

{ Senet Rules of this Implementation

Board:
+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
|     |     |     |     |     |     |     |     |     |     |
|  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  10 |
|     |     |     |     |     |     |     |     |     |     |
+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
|     |     |     |     |     |     |     |     |     |     |
|  20 |  19 |  18 |  17 |  16 |  O  |  14 |  13 |  12 |  11 |
|     |     |     |     |     |  +  |     |     |     |     |
+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
|     |     |     |     |     |     | ~~~ |     |     |     |
|  21 |  22 |  23 |  24 |  25 |  X  | ~~~ | III |  II |  30 |
|     |     |     |     |     |     | ~~~ |     |     |     |
+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+

15: House of Rebirth
27: House of Water

Start: Each player has N pieces, initially placed alternating on
  squares 1 to 2*N.

Move: Players alternate throwing a set of 4 two sided paddles to move
  foreward:
    1x white: move 1 square and throw again
    2x white: move 2 squares
    3x white: move 3 squares
    4x white: move 4 squares and throw again
    4x black: move 6 squares and throw again

Attack: Landing on an opponents' piece is an attack, and you exchange
  places; you may not land on your own pieces.

Restrictions:
  * First Move: First throw of the game must move the piece on square
      2*N where N is the number of pieces per player.
  * Safety: Squares 15, 26, 28, 29 cannot be attacked
  * Defence: Two or more consecutive opponent pieces cannot be attacked
  * Blockade: Three or more consecutive opponent pieces cannot be passed;
      however, blockades may not turn around corners (10 t0 11, 20 to 21)
  * Trap: Land on 20 and go back to 15 (or the first empty square before it)
  * Exit: You may not move past 30. A piece on 30 can be removed at the
    *start* of your turn if all your pieces are out of the first row.
  * No Move: If you cannot move foreward, you must move backward
    (according to the same rules). If no move is possible your turn ends.

Win: You win by removing all of your pieces.
}

unit uSenetGame;

{$mode objfpc}{$H+}

interface

{uses
  Classes, SysUtils;}
uses
  math;

type
  TPieceCount = 3..7;

  TPlayer = (pNone, pPlayer1, pPlayer2);

  TSquareNumber = 0..29;
  TSquareNumberSet = set of TSquareNumber;
  TPiece = TPlayer;

  TMove = record
    Start, Target: TSquareNumber;
    Moves: ShortInt;
    Player: TPlayer;
  end;

  TGameState = record
    TurnCount: Word;
    Board: Array[TSquareNumber] of TPiece;

    CurrentPlayer: TPlayer;
    CurrentMove: TMove;
    TurnFinished: Boolean;

    PiecesPerPlayer: TPieceCount;
    PiecesOffBoard: Array[pPlayer1..pPlayer2] of Byte;
  end;

  function GetEnemy(Player: TPlayer): TPlayer;

  function IsBlocked(State: TGameState; Move: TMove): Boolean;
  function CanAttack(State: TGameState; Square: TSquareNumber;
    Player: TPlayer): Boolean;

  function IsValidMove(State: TGameState; Move: TMove): Boolean;
  procedure SetStone(var AState: TGameState; Square: TSquareNumber;
    Piece: TPiece);

  function DoMove(var State: TGameState): Boolean;
  function DoMove(var State: TGameState; var Move: TMove): Boolean;
  procedure UndoMove(var State: TGameState; Move: TMove);
  procedure HandleHouseOfWater(var State: TGameState; var Move: TMove);

  function RemovePieceFromSquare30(var State: TGameState): Boolean;

  function SearchPossibleMove(var State: TGameState; var Move: TMove): Boolean;
  function SearchNextPossibleMove(State: TGameState; var Move: TMove): Boolean;

  function ThrowPaddles: Byte;

  procedure BeginTurn(var State: TGameState);
  procedure EndTurn(var State: TGameState);
  procedure InitGame(var State: TGameState; Pieces: TPieceCount);

  function HasWon(State: TGameState; Player: TPlayer): Boolean;

const
  SQUARE_REPEATING_LIFE: TSquareNumber = 14;
  SQUARE_V: TSquareNumber = 25;
  SQUARE_WATER: TSquareNumber = 26;
  SQUARE_III: TSquareNumber = 27;
  SQUARE_II: TSquareNumber = 28;
  SQUARE_I: TSquareNumber = 29;

  PROTECTED_SQUARES = [14, 25, 27, 28]; // Using SQUARE_* does not compile...
  PAST_CORNER_SQUARES = [10, 20];

implementation

procedure BeginTurn(var State: TGameState);
begin
  if State.TurnFinished then
  begin
    State.CurrentMove.Moves := ThrowPaddles;
    State.CurrentMove.Start := 0;
    State.CurrentMove.Player := State.CurrentPlayer;

    State.TurnFinished := False;
  end;
end;

procedure EndTurn(var State: TGameState);
begin
  // Do not end turn if no piece has moved
  if not State.TurnFinished then Exit;

  WriteLn('NEXT!');
  // determine next player
  case State.CurrentMove.Moves of
    6, 1, 4: State.CurrentPlayer := State.CurrentPlayer;
    else State.CurrentPlayer := GetEnemy(State.CurrentPlayer);
  end;

  Inc(State.TurnCount);
end;

function GetEnemy(Player: TPlayer): TPlayer;
begin
  case Player of
    pPlayer1: Result := pPlayer2;
    pPlayer2: Result := pPlayer1;
    pNone: Result := pNone;
  end;
end;

{ Returns True if there is a blockade on the way to the target square.
  False otherwise. }
function IsBlocked(State: TGameState; Move: TMove): Boolean;
var
  i, StartSquare, EndSquare: TSquareNumber;
  Count: Byte;
begin
  Result := False;
  Count := 0;


  if Move.Moves > 0 then
  begin
    StartSquare := Max(Low(TSquareNumber), Move.Start + 1);
    EndSquare := Min(High(TSquareNumber), Move.Start + Move.Moves);
  end
  else
  begin
    StartSquare := Max(Low(TSquareNumber), Move.Start - 1);
    EndSquare := Min(High(TSquareNumber), Move.Start + Move.Moves);
  end;

  for i := StartSquare to EndSquare do
  begin
    // "Blockades may not turn around corners"
    if i in PAST_CORNER_SQUARES then Count := 0;

    if State.Board[i] = GetEnemy(Move.Player) then
    begin
      Inc(Count);
      if Count >= 3 then Exit(True);
    end
    else Count := 0;
  end;
end;

{ Returns True if player Player cannot attack a piece on the specified square. }
function CanAttack(State: TGameState; Square: TSquareNumber;
  Player: TPlayer): Boolean;
var
  Field: TSquareNumber;
  Count: Byte;
begin
  Result := True;

  if State.Board[Square] = pNone then Exit(True);
  // "Squares [...] cannot be attacked"
  if Square in PROTECTED_SQUARES then Exit(False);
  // "you may not land on your own pieces"
  if State.Board[Square] = Player then Exit(False);

  Count := 0;
  for Field := Max(Low(TSquareNumber), Square - 1) to
    Min(High(TSquareNumber), Square + 1) do
  begin
    if State.Board[Field] = GetEnemy(Player) then
    begin
      Inc(Count);

      if Count >= 2 then Exit(False);
    end
    else Count := 0;
  end;
end;

function IsValidMove(State: TGameState; Move: TMove): Boolean;
var
  Target: Integer;
begin
  if State.TurnCount = 0 then
  begin
    Result := Move.Start = Low(TSquareNumber) + 2*State.PiecesPerPlayer - 1;
  end
  else
  begin
    Target := Move.Start + Move.Moves;
    // "You may not pass past square 30"
    // On the target field may not stand an own piece
    // The target square must be attackable
    // There may not be a blockade on the way to the target square
    Result := ((Target <= High(TSquareNumber)) and
               (Target >= Low(TSquareNumber)) and
               (State.Board[Target] <> Move.Player) and
               CanAttack(State, Target, Move.Player) and
               not IsBlocked(State, Move));
  end;
end;

procedure SetStone(var AState: TGameState; Square: TSquareNumber; Piece: TPiece);
begin
  AState.Board[Square] := Piece;
end;

function DoMove(var State: TGameState): Boolean;
begin
  DoMove(State, State.CurrentMove);
end;

function DoMove(var State: TGameState; var Move: TMove): Boolean;
var
  //Target: TSquareNumber;
  TargetPiece: TPiece;
begin
  if IsValidMove(State, State.CurrentMove) then
  begin
    Move.Target := Move.Start + Move.Moves;

    // Swap stones if target square is not empty
    // Move stone if target square is empty
    TargetPiece := State.Board[Move.Target];
    SetStone(State, Move.Target, Move.Player);
    SetStone(State, Move.Start, TargetPiece);

    Result := True;

    // Handle special squares
    if Move.Target = SQUARE_WATER then
    begin
      HandleHouseOfWater(State, Move);
    end;
  end
  else
  begin
    // Return error
    Result := False;
  end;

  State.TurnFinished := Result;
end;

function RemovePieceFromSquare30(var State: TGameState): Boolean;
var
  Square: TSquareNumber;
begin
  Result := False;

  if (not State.TurnFinished) and (State.Board[29] = State.CurrentPlayer) then
  begin
    for Square := Low(TSquareNumber) to 9 do
    begin
      if State.Board[Square] = State.CurrentPlayer then
      begin
        Exit(False);
      end;
    end;
  
    SetStone(State, 29, pNone);
    Inc(State.PiecesOffBoard[State.CurrentPlayer]);
    Result := True;
  end;
end;

function SearchPossibleMove(var State: TGameState; var Move: TMove): Boolean;
var
  i: TSquareNumber;
  OldStart: TSquareNumber;
begin
  Result := False;
  OldStart := Move.Start;

  Move.Start := Low(TSquareNumber);
  if SearchNextPossibleMove(State, Move) then Exit(True);

  Move.Moves := -Move.Moves;
  if SearchNextPossibleMove(State, Move) then Exit(True);

  // No possible move found...
  Move.Start := OldStart;
  Move.Moves := -Move.Moves;
  State.TurnFinished := True;
end;

function SearchNextPossibleMove(State: TGameState; var Move: TMove): Boolean;
var
  Square: TSquareNumber;
  OldStart: TSquareNumber;
begin
  OldStart := Move.Start;
  Result := False;

  for Square := Move.Start to High(TSquareNumber) do
  begin
    if State.Board[Square] = Move.Player then
    begin
      Move.Start := Square;
      if IsValidMove(State, Move) then
      begin
        Exit(True);
      end;
    end;
  end;

  // Nothing found reset move
  Move.Start := OldStart;
end;

procedure UndoMove(var State: TGameState; Move: TMove);
var
  TargetPiece: TPiece;
begin
  // Just swap stones again to undo move.
  TargetPiece := State.Board[Move.Target];
  State.Board[Move.Target] := State.Board[Move.Start];
  State.Board[Move.Start] := TargetPiece;

  // No checking for special fields.

  State.TurnFinished := False;
end;

procedure HandleHouseOfWater(var State: TGameState; var Move: TMove);
var
  i: Integer;
begin
  // Move piece to an empty square at or before the house of rebirth.
  for i := SQUARE_REPEATING_LIFE downto Low(TSquareNumber) do
  begin
    if State.Board[i] = pNone then
    begin
      State.Board[i] := State.Board[SQUARE_WATER];
      Move.Target := i;
      Break;
    end;
  end;

  State.Board[SQUARE_WATER] := pNone;
end;

function ThrowPaddles: Byte;
var
  i: Byte;
begin
  Result := 0;

  for i := 0 to 4 - 1 do
  begin
    if Random(2) = 1 then Inc(Result);
  end;

  if Result = 0 then Result := 6;
end;

{ Initiates a game. Player1 and Player2 stones are placed on the
  board in alternating order. Every player gets Stones pieces. }
procedure InitGame(var State: TGameState; Pieces: TPieceCount);
var
  i: Byte;
begin
  for i := 0 to Pieces - 1 do
  begin
    SetStone(State, 2*i, pPlayer1);
    SetStone(State, 2*i+1, pPlayer2);
  end;

  for i := 2 * Pieces to High(State.Board) do
  begin
    SetStone(State, i, pNone);
  end;

  State.TurnCount := 0;
  State.TurnFinished := True;
  State.CurrentPlayer := pPlayer2;

  State.PiecesPerPlayer := Pieces;
  State.PiecesOffBoard[pPlayer1] := 0;
  State.PiecesOffBoard[pPlayer2] := 0;
end;

function HasWon(State: TGameState; Player: TPlayer): Boolean;
begin
  Result := State.PiecesOffBoard[Player] = State.PiecesPerPlayer;
end;

initialization

Randomize;

end.

