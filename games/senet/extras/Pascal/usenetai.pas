{
  uSenetAI.pas
  Simple AI for Senet.
  

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

unit uSenetAI;

{$mode objfpc}{$H+}

interface

uses
  uSenetGame;

  function StateValue(State: TGameState; Player: TPlayer): Integer;

  function GetBestMove(var State: TGameState): Boolean;

  //function MinMax(var State: TGameState; Player: TPlayer; Depth: Byte): Integer;
  //function AlphaBetaCut(var State: TGameState; Player: TPlayer; Depth: Byte;
  //  Alpha, Beta: Integer): Integer;

implementation

function StateValue(State: TGameState; Player: TPlayer): Integer;
var
  Square: TSquareNumber;
  Value: Integer;
  LastPiece: TPiece;
  PiecesInARowCount: Byte;
begin
  { A piece off board is very worthy.
    A piece on field 30 is worthy.
    A piece on any secure field is rather worthy.
    A blockade is worthier than a defence.
    A defence is worthier than a single piece.
    A piece in row 3 is worthier than in 2 than in 1.
  }
  //Result := 15 * (State.PiecesOffBoard[Player] - State.PiecesOffBoard[GetEnemy(Player)]);
  Result := 0;
  LastPiece := pNone;

  for Square := Low(TSquareNumber) to High(TSquareNumber) do
  begin
    if (State.Board[Square] = LastPiece) and (LastPiece <> pNone) and
      not (Square in PAST_CORNER_SQUARES) then
    begin
      Inc(PiecesInARowCount);
    end
    else
    begin
      PiecesInARowCount := 0;
    end;
    LastPiece := State.Board[Square];

    case Square of
      14, 25, 27, 28: Value := 3*Square;

      0..9: Value := Square;
      10..13, 15..19: Value := Square;
      20..24, 26: Value := Square;
      29: Value := 10*Square;
    end;

    case PiecesInARowCount of
      //0..1: ;
      2: Inc(Value, Value);
      3: Inc(Value, 2*Value);
      else
        //Dec(Value, Value);
    end;

    if LastPiece = Player then Inc(Result, Value)
    else if LastPiece = GetEnemy(Player) then Dec(Result, Value);
  end;
end;

function GetBestMove(var State: TGameState): Boolean;
var
  BestValue, Value: Integer;
  BestStart: TSquareNumber;
begin
  if SearchPossibleMove(State, State.CurrentMove) then
  begin
    BestValue := Low(Integer);

    repeat
      DoMove(State, State.CurrentMove);
      Value := StateValue(State, State.CurrentPlayer) - StateValue(State, GetEnemy(State.CurrentPlayer));
      if Value > BestValue then
      begin
        BestValue := Value;
        BestStart := State.CurrentMove.Start;
      end;
      UndoMove(State, State.CurrentMove);
      Inc(State.CurrentMove.Start);
    until not SearchNextPossibleMove(State, State.CurrentMove);

    WriteLn('Best Value: ', BestValue);
    State.CurrentMove.Start := BestStart;
    Result := True;
  end
  else Result := False;
end;

function MinMax(var State: TGameState; Player: TPlayer; Depth: Byte): Integer;
var
  Move: TMove;
begin
  if Depth = 0 then Exit(StateValue(State, Player));

  Move.Start := Low(TSquareNumber);
  Move.Moves := 3;

  while SearchNextPossibleMove(State, Move) do
  begin

  end;
end;

function AlphaBetaCut(var State: TGameState; Player: TPlayer; Depth: Byte;
  Alpha, Beta: Integer): Integer;
begin
  if Depth = 0 then Exit(StateValue(State, Player));

  // TODO
end;

end.

