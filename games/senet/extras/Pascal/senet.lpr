{
  A Senet game on the console for porting it to the Gamebuino.
  Senet is an old egyptian board game.
  
  
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

program senet;

uses sysutils, uSenetGame, uSenetAI, Crt;

procedure PrintBoard(AState: TGameState);
var
  Row, Column: TSquareNumber;
begin
  for Row := 0 to 2 do
  begin
    Write('|');
    if Row = 1 then  // Row 1 is drawn from right to left
    begin
      for Column := (Row + 1) * 10 - 1 downto Row * 10 do
      begin
        case AState.Board[Column] of
          pNone: Write(' ');
          pPlayer1: Write('A');
          pPlayer2: Write('B');
        end;
        Write('|');
      end;
    end
    else
    begin
      for Column := Row * 10 to (Row + 1) * 10 - 1 do
      begin
        case AState.Board[Column] of
          pNone: Write(' ');
          pPlayer1: Write('A');
          pPlayer2: Write('B');
        end;
        Write('|');
      end;
    end;
    WriteLn();
  end;
end;

var
  State: TGameState;

  Input: String;
  SkipCount: Byte;
begin
  InitGame(State, 7);

  BeginTurn(State);
  Input := '';
  while not (HasWon(State, pPlayer1) or HasWon(State, pPlayer2)) and
    (SkipCount < 2) do
  begin
    ClrScr;

    Write('Player: ');
    case State.CurrentPlayer of
      pPlayer1: WriteLn('1');
      pPlayer2: WriteLn('2');
    end;

    PrintBoard(State);
    WriteLn();

    WriteLn('Moves: ' + IntToStr(State.CurrentMove.Moves));

    if RemovePieceFromSquare30(State) then
    begin
      WriteLn('Removing piece from square 30.');
    end
    else
    begin
      WriteLn();
    end;

    if GetBestMove(State) then
    begin
      {WriteLn('There is a possible move. (' +
        IntToStr(State.CurrentMove.Start) +
        ' to ' +
        IntToStr(State.CurrentMove.Start + State.CurrentMove.Moves) + ')');

      if IsValidMove(State, State.CurrentMove) then
        WriteLn(' -> Move valid.')
      else
        WriteLn(' -> MOVE INVALID!');    }
      WriteLn();

      DoMove(State);
      EndTurn(State);
      BeginTurn(State);

      SkipCount := 0;
    end
    else
    begin
      Inc(SkipCount);

      WriteLn('There is no possible move! Skipping...');
      EndTurn(State);
      BeginTurn(State);
    end;

    WriteLn();
    PrintBoard(State);

    WriteLn();
    Write('>>> ');
    ReadLn(Input);

    if lowercase(Input) = 'x' then
    begin
      break;
    end;
  end;
end.

