#include <SPI.h>
#include <Gamebuino.h>
#include <EEPROM.h>
Gamebuino gb;

extern const byte font3x3[];
extern const byte font3x5[];
extern const byte font5x7[];

const char GAME_ID[] PROGMEM = "2048 GAMEBUINO";

struct Gamestate {
  char id[sizeof(GAME_ID)];
  int board2048[16];
  long score;
  long highscore;
  bool winstate;
} gameState;

const char msg8[] PROGMEM = "8! Good!";
const char msg16[] PROGMEM = "16! Great!";
const char msg32[] PROGMEM = "32! Awesome!";
const char msg64[] PROGMEM = "64! Sweet!";
const char msg128[] PROGMEM = "128! Cool!";
const char msg256[] PROGMEM = "256! Keep it up!";
const char msg512[] PROGMEM = "512! Almost there!";
const char msg1024[] PROGMEM = "1024! One more!";
const char msg2048[] PROGMEM = "2048! You win!";
const char msg4096[] PROGMEM = "4096! Step it up!";
const char msg8192[] PROGMEM = "8192! You're good!";
const char msg16384[] PROGMEM = "16384! Keep playing!";
const char msg32768[] PROGMEM = "32768! Unbelievable!";
const char msg65536[] PROGMEM = "65536! Woohoo!";
const char msg131072[] PROGMEM = "131072! INSANE!!!";

const char* const newTileStrings[] PROGMEM = {msg8, msg16, msg32, msg64, msg128, msg256, msg512, msg1024, msg2048, msg4096, msg8192, msg16384, msg32768, msg65536, msg131072};

const byte Title2048[] PROGMEM = {64,30,
0x0,0x0,0xFF,0xFF,0xFF,0xFC,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x2,0x0,0x0,0x0,0x1,0x3C,0x78,0x38,0xF2,0x0,0x0,0x0,0x1,0x7E,0xFC,0x79,0xFA,0x0,0x0,
0x0,0x1,0x66,0xCC,0xD9,0x9A,0x0,0x0,0x0,0x1,0x6,0xCD,0x99,0xFA,0x0,0x0,0x0,0x1,0xE,0xCD,0xFC,0xF2,0x0,0x0,0x0,0x1,0x1C,0xCD,0xFD,0xFA,0x0,0x0,
0x0,0x1,0x38,0xCC,0x19,0x9A,0x0,0x0,0x0,0x1,0x7E,0xFC,0x19,0xFA,0x0,0x0,0x0,0x1,0x7E,0x78,0x18,0xF2,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x2,0x0,0x0,
0x0,0x0,0xFF,0xFF,0xFF,0xFC,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xEA,0xE7,0x73,0x73,0xA2,0x57,0x60,0x0,0x4E,0xC7,0x52,0x23,0xA7,0x26,0x50,0x0,
0x4A,0xE5,0x76,0x22,0x35,0x27,0x60,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6,0x27,0x73,0xB8,0x7,0xFF,0xFF,0xE0,0x5,0x77,0x62,0xB0,0x8,0x0,0x0,0x10,
0x7,0x55,0x73,0xA0,0x8,0xDF,0xF8,0x10,0x0,0x0,0x0,0x0,0x8,0xDF,0xF8,0x10,0x0,0x62,0x23,0x0,0x8,0x10,0x8,0x10,0x0,0x15,0x65,0x0,0x8,0x10,0x8,0x10,
0x0,0x25,0x27,0x80,0x8,0x90,0x8,0xD0,0x0,0x72,0x71,0x0,0x9,0x50,0x8,0xD0,0x0,0x0,0x0,0x0,0x8,0x90,0xB,0x10,0x0,0x0,0x0,0x0,0x8,0xF,0xF3,0x10,
0x0,0x0,0x0,0x0,0x6,0x0,0x0,0x60,0x0,0x0,0x0,0x0,0x1,0xFF,0xFF,0x80,
};

const byte Logo2048[] PROGMEM = {24,6,
B01110001,B11001001,B10001110,
B10011010,B01101001,B10010011,
B00011010,B01101001,B10001110,
B00110010,B01101111,B11010011,
B01100010,B01100001,B10010011,
B11111001,B11000001,B10001110
};

const byte NewTile_1[] PROGMEM = {13,12,
B11111111,B11111000,
B10000000,B00001000,
B10000000,B00001000,
B10000000,B00001000,
B10001111,B10001000,
B10001000,B10001000,
B10001000,B10001000,
B10001111,B10001000,
B10000000,B00001000,
B10000000,B00001000,
B10000000,B00001000,
B11111111,B11111000
};

const byte NewTile_2[] PROGMEM = {13,12,
B11111111,B11111000,
B10000000,B00001000,
B10000000,B00001000,
B10011111,B11001000,
B10010000,B01001000,
B10010000,B01001000,
B10010000,B01001000,
B10010000,B01001000,
B10011111,B11001000,
B10000000,B00001000,
B10000000,B00001000,
B11111111,B11111000
};

const byte NewTile_3[] PROGMEM = {13,12,
B11111111,B11111000,
B10000000,B00001000,
B10111111,B11101000,
B10100000,B00101000,
B10100000,B00101000,
B10100111,B00101000,
B10100111,B00101000,
B10100000,B00101000,
B10100000,B00101000,
B10111111,B11101000,
B10000000,B00001000,
B11111111,B11111000
};

const byte Tile_0[] PROGMEM = {13,12,
B11111111,B11111000,
B10000000,B00001000,
B10000000,B00001000,
B10000000,B00001000,
B10000000,B00001000,
B10000000,B00001000,
B10000000,B00001000,
B10000000,B00001000,
B10000000,B00001000,
B10000000,B00001000,
B10000000,B00001000,
B11111111,B11111000
};

const byte Tile_2[] PROGMEM = {13,12,
B11111111,B11111000,
B10000000,B00001000,
B10001111,B10001000,
B10010000,B01001000,
B10000000,B01001000,
B10000011,B10001000,
B10001100,B00001000,
B10010000,B00001000,
B10010000,B00001000,
B10011111,B11001000,
B10000000,B00001000,
B11111111,B11111000
};

const byte Tile_4[] PROGMEM = {13,12,
B11111111,B11111000,
B10000000,B00001000,
B10000011,B10001000,
B10000100,B10001000,
B10001000,B10001000,
B10010000,B10001000,
B10011111,B11001000,
B10000000,B10001000,
B10000000,B10001000,
B10000000,B10001000,
B10000000,B00001000,
B11111111,B11111000
};

const byte Tile_8[] PROGMEM = {13,12,
B11111111,B11111000,
B10000000,B00001000,
B10001111,B10001000,
B10010000,B01001000,
B10010000,B01001000,
B10001111,B10001000,
B10010000,B01001000,
B10010000,B01001000,
B10010000,B01001000,
B10001111,B10001000,
B10000000,B00001000,
B11111111,B11111000
};

const byte Tile_16[] PROGMEM = {13,12,
B11111111,B11111000,
B10000000,B00001000,
B10010011,B11001000,
B10110100,B00101000,
B10010100,B00001000,
B10010111,B11001000,
B10010100,B00101000,
B10010100,B00101000,
B10010100,B00101000,
B10010011,B11001000,
B10000000,B00001000,
B11111111,B11111000
};

const byte Tile_32[] PROGMEM = {13,12,
B11111111,B11111000,
B10000000,B00001000,
B10011000,B11001000,
B10100101,B00101000,
B10000100,B00101000,
B10001000,B00101000,
B10000100,B11001000,
B10000101,B00001000,
B10100101,B00001000,
B10011001,B11101000,
B10000000,B00001000,
B11111111,B11111000
};

const byte Tile_64[] PROGMEM = {13,12,
B11111111,B11111000,
B10000000,B00001000,
B10011000,B01001000,
B10100100,B11001000,
B10100001,B01001000,
B10111001,B11101000,
B10100100,B01001000,
B10100100,B01001000,
B10100100,B01001000,
B10011000,B01001000,
B10000000,B00001000,
B11111111,B11111000
};

const byte Tile_128[] PROGMEM = {13,12,
B11111111,B11111000,
B10000000,B00001000,
B10101100,B01001000,
B10100010,B10101000,
B10100010,B10101000,
B10100100,B01001000,
B10101000,B10101000,
B10101000,B10101000,
B10101000,B10101000,
B10101110,B01001000,
B10000000,B00001000,
B11111111,B11111000
};

const byte Tile_256[] PROGMEM = {13,12,
B11111111,B11111000,
B10000000,B00001000,
B10110110,B01101000,
B10010100,B10001000,
B10010100,B10001000,
B10010110,B11001000,
B10100010,B10101000,
B10100010,B10101000,
B10100010,B10101000,
B10110110,B01001000,
B10000000,B00001000,
B11111111,B11111000
};

const byte Tile_512[] PROGMEM = {13,12,
B11111111,B11111000,
B10000000,B00001000,
B10110010,B11001000,
B10100110,B00101000,
B10100010,B00101000,
B10100010,B01001000,
B10010010,B10001000,
B10010010,B10001000,
B10010010,B10001000,
B10110010,B11101000,
B10000000,B00001000,
B11111111,B11111000
};

const byte Tile_1024[] PROGMEM = {13,12,
B11111111,B11111000,
B10000000,B00001000,
B10001011,B10001000,
B10001010,B10001000,
B10001011,B10001000,
B10000000,B00001000,
B10011010,B10001000,
B10001010,B10001000,
B10010011,B11001000,
B10011000,B10001000,
B10000000,B00001000,
B11111111,B11111000
};

const byte Tile_2048[] PROGMEM = {13,12,
B11111111,B11111000,
B10000000,B00001000,
B10011001,B11001000,
B10001001,B01001000,
B10001101,B11001000,
B10000000,B00001000,
B10010101,B11001000,
B10010101,B11001000,
B10011101,B01001000,
B10000101,B11001000,
B10000000,B00001000,
B11111111,B11111000
};

const byte Tile_4096[] PROGMEM = {13,12,
B11111111,B11111000,
B10000000,B00001000,
B10010101,B11001000,
B10011101,B01001000,
B10000101,B11001000,
B10000000,B00001000,
B10011101,B00001000,
B10010101,B11001000,
B10011101,B01001000,
B10000101,B11001000,
B10000000,B00001000,
B11111111,B11111000
};

const byte Tile_8192[] PROGMEM = {13,12,
B11111111,B11111000,
B10000000,B00001000,
B10001100,B10001000,
B10001110,B10001000,
B10000110,B10001000,
B10000000,B00001000,
B10011101,B10001000,
B10010100,B01001000,
B10011100,B10001000,
B10000101,B11001000,
B10000000,B00001000,
B11111111,B11111000
};

const byte Tile_16384[] PROGMEM = {13,12,
B11111111,B11111000,
B10000000,B00001000,
B10101000,B11101000,
B10101110,B01101000,
B10101110,B11101000,
B10000000,B00001000,
B10011101,B01001000,
B10011101,B11001000,
B10010100,B01001000,
B10011100,B01001000,
B10000000,B00001000,
B11111111,B11111000
};

const byte Tile_32768[] PROGMEM = {13,12,
B11111111,B11111000,
B10000000,B00001000,
B10111011,B01101000,
B10011001,B00101000,
B10001010,B01001000,
B10111011,B01001000,
B10000000,B00001000,
B10010001,B10001000,
B10011101,B11001000,
B10011100,B11001000,
B10000000,B00001000,
B11111111,B11111000
};

const byte Tile_65536[] PROGMEM = {13,12,
B11111111,B11111000,
B10000000,B00001000,
B10100011,B01101000,
B10111010,B01001000,
B10101001,B00101000,
B10111011,B01101000,
B10000000,B00001000,
B10011100,B11001000,
B10001100,B10001000,
B10011101,B10001000,
B10000000,B00001000,
B11111111,B11111000
};

const byte Tile_131072[] PROGMEM = {13,12,
B11111111,B11111000,
B10000000,B00001000,
B10010111,B01001000,
B10010011,B01001000,
B10010111,B01001000,
B10000000,B00001000,
B10010111,B01101000,
B10101001,B00101000,
B10101010,B01001000,
B10010010,B01101000,
B10000000,B00001000,
B11111111,B11111000
};

const byte *TileSprites[] = {
Tile_0,Tile_2,Tile_4,Tile_8,Tile_16,Tile_32,Tile_64,Tile_128,Tile_256,Tile_512,Tile_1024,Tile_2048,Tile_4096,Tile_8192,Tile_16384,Tile_32768,Tile_65536,Tile_131072
};

int Board2048[16];

int Board2048Old[16];

int TempBoard[16];

int TempBoard2[16];

int RotArray[] = {
12,8,4,0,
13,9,5,1,
14,10,6,2,
15,11,7,3
};

void setup() {
  gb.begin();
  gb.titleScreen(F("JWinslow23 presents"), Title2048);
  gb.display.persistence = true;
  ResetDisplay();
  gb.battery.show = false;
  gb.pickRandomSeed();
  if(isValidGame()) {
    restoreGame();
    gb.popup(F("Back already?"),40);
  } else {
    newSave(false);
    gb.popup(F("Welcome to 2048!"),40);
  }
}

void loop() {
  if(gb.update()) {
    DrawBoard();
    for( int x = 0; x < 16; x++ ) {
      Board2048Old[x] = Board2048[x];
    }
    bool ButtonPressed = false;
    if(gb.buttons.pressed(BTN_LEFT)) {
      ButtonPressed = true;
      RotateClockwise();
      RotateClockwise();
      MoveRight(true);
      RotateClockwise();
      RotateClockwise();
    }
    if(gb.buttons.pressed(BTN_RIGHT)) {
      ButtonPressed = true;
      MoveRight(true);
    }
    if(gb.buttons.pressed(BTN_UP)) {
      ButtonPressed = true;
      RotateClockwise();
      MoveRight(true);
      RotateClockwise();
      RotateClockwise();
      RotateClockwise();
    }
    if(gb.buttons.pressed(BTN_DOWN)) {
      ButtonPressed = true;
      RotateClockwise();
      RotateClockwise();
      RotateClockwise();
      MoveRight(true);
      RotateClockwise();
    }
    PopupMessage();
    int y = 0;
    bool WinBox = false;
    int TilesOnBoard = 0;
    for( int x = 0; x < 16; x++ ) {
      if (Board2048[x]) TilesOnBoard++;
      if (Board2048Old[x] == Board2048[x]) y++;
      if (Board2048[x] == 11 && gameState.winstate == false) {
        gameState.winstate = true;
        WinBox = true;
      }
    }
    if (y != 16) {
      SpawnTile(true);
    } else if (ButtonPressed) {
      gb.sound.playCancel();
    }
    if (TilesOnBoard == 15 && y != 16) {
      for( int x = 0; x < 16; x++ ) {
        TempBoard2[x] = Board2048[x];
      }
      MoveRight(false);
      RotateClockwise();
      MoveRight(false);
      RotateClockwise();
      RotateClockwise();
      RotateClockwise();
      int y = 0;
      for( int x = 0; x < 16; x++ ) {
        if (Board2048[x] == TempBoard2[x]) y++;
      }
      if (y == 16) {
        DrawLoseBox();
        while(gb.buttons.pressed(BTN_B) == false) {
          DrawLoseBox();
          gb.update();
        }
        do {
          if(gb.update()) {
            DrawLoseBox();
            if(gb.buttons.pressed(BTN_B)) {
              newGame(true);
              gb.popup(F("Game reset."),40);
            }
          }
        } while(gb.buttons.repeat(BTN_B,1) == false);
      } else {
        for( int x = 0; x < 16; x++ ) {
          Board2048[x] = TempBoard2[x];
        }
      }
    }
    if (WinBox) {
      DrawWinBox();
      while(gb.buttons.repeat(BTN_A,1) == true || gb.buttons.repeat(BTN_B,1) == true) {
        DrawWinBox();
        gb.update();
      }
      int ButtonAB = 0;
      do {
        if(gb.update()) {
          DrawWinBox();
          if(gb.buttons.pressed(BTN_A)) {
            ButtonAB = 1;
            gb.sound.playOK();
            gb.popup(F("Game continued."),40);
          }
          if(gb.buttons.pressed(BTN_B)) {
            ButtonAB = 2;
            newGame(true);
            gb.popup(F("Game reset."),40);
          }
        }
      } while(ButtonAB == 0);      
    }
    
    if(gb.buttons.pressed(BTN_A) && WinBox == false) {
      saveGame();
    }
    if(gb.buttons.repeat(BTN_B,1)) {
      bool APressed = false;
      while(gb.buttons.released(BTN_B) == false && gb.buttons.pressed(BTN_A) == false ) {
        DrawBoard();
        gb.update();
        if(gb.buttons.pressed(BTN_A)) APressed = true;
      }
      if(APressed == false) {
        newGame(true);
        gb.popup(F("Game reset."),40);
      } else {
        DrawSaveBox();
        while(gb.buttons.released(BTN_B) == false) {
          DrawSaveBox();
          gb.update();
        }
        int ButtonAB = 0;
        do {
          if(gb.update()) {
            DrawSaveBox();
            if(gb.buttons.pressed(BTN_A)) {
              ButtonAB = 1;
              newSave(true);
              gb.popup(F("Save deleted."),40);
            }
            if(gb.buttons.pressed(BTN_B)) {
              ButtonAB = 2;
              while(gb.buttons.released(BTN_B) == false ) {
                gb.update();
              }
              gb.sound.playOK();
              gb.popup(F("Deletion cancelled."),40);
            }
          }
        } while(ButtonAB == 0);
      }
    }
    if(gb.buttons.pressed(BTN_C)) {
      saveGame();
      gb.titleScreen(F("JWinslow23 presents"), Title2048);
      gb.display.persistence = true;
      ResetDisplay();
      gb.battery.show = false;
      gb.pickRandomSeed();
      if(isValidGame()) {
        restoreGame();
        gb.popup(F("Back already?"),40);
      } else {
        newSave(false);
        gb.popup(F("Welcome to 2048!"),40);
      }
    }
  }
}

void newSave(bool makeSound) {
  strcpy_P(gameState.id, GAME_ID);
  for( int x = 0; x < 16; x++ ) {
    gameState.board2048[x] = 0;
  }
  gameState.score = 0L;
  gameState.highscore = 0L;
  gameState.winstate = false;
  newGame(makeSound);
}

void newGame(bool makeSound) {
  gameState.score = 0L;
  ResetDisplay();
  for( int x = 0; x < 16; x++ ) {
    Board2048[x] = 0;
  }
  SpawnTile(makeSound);
  SpawnTile(false);
  gameState.winstate = false;
}

void saveGame() {
  for (int x = 0; x < 16; x++) {
    gameState.board2048[x] = Board2048[x];
  }
  for (int x = 0; x < sizeof(gameState); x++) {
    EEPROM.write(x, ((uint8_t*)&gameState)[x]);
  }
  gb.sound.playOK();
  gb.popup(F("Game saved."),40);
}

void restoreGame() {
  for (int x = 0; x < sizeof(gameState); x++) {
    ((uint8_t*)&gameState)[x] = EEPROM.read(x);
  }
  for (int x = 0; x < 16; x++) {
    Board2048[x] = gameState.board2048[x];
  }
}

boolean isValidGame() {
  char id[sizeof(GAME_ID)];
  for (int x = 0; x < sizeof(id); x++) 
    ((uint8_t*)&id)[x] = EEPROM.read(x);
  return strcmp_P(id, GAME_ID) == 0;
}

void SpawnTile(bool makeSound) {
  int RandTile;
  do {
    RandTile = random(16);
  } while(Board2048[RandTile] != 0);
  
  for( int x = 0; x < 3; x++ ) {
    DrawBoard();
    if (x == 0) gb.display.drawBitmap(RandTile % 4 * 13, RandTile / 4 * 12, NewTile_1);
    if (x == 1) gb.display.drawBitmap(RandTile % 4 * 13, RandTile / 4 * 12, NewTile_2);
    if (x == 2) gb.display.drawBitmap(RandTile % 4 * 13, RandTile / 4 * 12, NewTile_3);
    while(gb.update() == false) {}
  }
  if (makeSound) gb.sound.playOK();
  if(random(10)) {
    Board2048[RandTile] = 1;
  } else {
    Board2048[RandTile] = 2;
  }
}

void MoveRight(bool Animate) {
  CompressRight();
  for( int x = 0; x < 16; x += 4 ) {
    for( int y = 3; y >= 1; y-- ) {
      if(Board2048[x + y] == Board2048[x + y - 1] && Board2048[x + y] != 0) {
        Board2048[x + y] += 1;
        long MergeScore = 1L;
        for( int z = 0; z < Board2048[x + y]; z++ ) {
          MergeScore *= 2;
        }
        if (Animate) gameState.score += MergeScore;
        Board2048[x + y - 1] = 0;
      }
    }
  }
  if (Animate) {
    if (gameState.score > gameState.highscore) gameState.highscore = gameState.score;
  }
  CompressRight();
}

void CompressRight() {
  for( int x = 0; x < 16; x += 4 ) {
    int FarthestTile = 4;
    for( int y = 0; y < 4; y++ ) {
      if(Board2048[x + y] == 0) FarthestTile = y;
    }
    for( int y = 3; y >= 0; y-- ) {
      if(Board2048[x + y] && y < FarthestTile && FarthestTile < 4) {
        Board2048[x + FarthestTile] = Board2048[x + y];
        Board2048[x + y] = 0;
        FarthestTile -= 1;
      }
    }
  }
}

void RotateClockwise() {
  for( int x = 0; x < 16; x++ ) {
    TempBoard[x] = Board2048[x];
  }
  for( int x = 0; x < 16; x++ ) {
    Board2048[x] = TempBoard[RotArray[x]];
  }
}

void DrawBoard() {
  ResetDisplay();
  for( int y = 0; y < 4; y++ ) {
    for( int x = 0; x < 4; x++ ) {
      gb.display.drawBitmap(x * 13, y * 12, TileSprites[Board2048[y * 4 + x]]);
    }
  }
}

void PopupMessage() {
  int MaxTile = Board2048[0];
  for( int x; x < 16; x++ ) {
    if (MaxTile < Board2048[x]) {
      MaxTile = Board2048[x];
    }
  }
  for( int x; x < 16; x++ ) {
    if (MaxTile == Board2048Old[x]) MaxTile = 0;
  }
  if (MaxTile >= 3) {
    gb.popup((const __FlashStringHelper*)pgm_read_word(&newTileStrings[MaxTile-3]),40);
  }
}

void ResetDisplay() {
  gb.display.clear();
  gb.display.drawBitmap(56,0,Logo2048);
  gb.display.drawRect(53,7,31,13);
  gb.display.drawRect(53,21,31,13);
  gb.display.cursorX = 55;
  gb.display.cursorY = 9;
  gb.display.print(gameState.score);
  gb.display.cursorX = 55;
  gb.display.cursorY = 23;
  gb.display.print(gameState.highscore);
  gb.display.setFont(font3x3);
  gb.display.cursorX = 55;
  gb.display.cursorY = 15;
  gb.display.print(F("POINTS"));
  gb.display.cursorX = 55;
  gb.display.cursorY = 29;
  gb.display.print(F("HIGH"));
  gb.display.setFont(font3x5);
  gb.display.cursorX = 53;
  gb.display.cursorY = 35;
  gb.display.print(F("\25:save"));
  gb.display.cursorX = 53;
  gb.display.cursorY = 42;
  gb.display.print(F("\26:reset"));
}

void DrawSaveBox() {
  DrawBoard();
  gb.display.drawRect(16,4,51,40);
  gb.display.setColor(WHITE);
  gb.display.fillRect(17,5,49,38);
  gb.display.setColor(BLACK);
  gb.display.cursorX = 18;
  gb.display.cursorY = 6;
  gb.display.print(F("ARE YOU SURE"));
  gb.display.cursorX = 18;
  gb.display.cursorY = 12;
  gb.display.print(F("YOU WANT TO"));
  gb.display.cursorX = 18;
  gb.display.cursorY = 18;
  gb.display.print(F("DELETE YOUR"));
  gb.display.cursorX = 18;
  gb.display.cursorY = 24;
  gb.display.print(F("HI-SCORE AND"));
  gb.display.cursorX = 18;
  gb.display.cursorY = 30;
  gb.display.print(F("SAVE DATA?"));
  gb.display.cursorX = 18;
  gb.display.cursorY = 36;
  gb.display.print(F("\25:yes   \26:no"));
}

void DrawWinBox() {
  DrawBoard();
  gb.display.drawRect(16,11,51,25);
  gb.display.setColor(WHITE);
  gb.display.fillRect(17,12,49,23);
  gb.display.setColor(BLACK);
  gb.display.cursorX = 18;
  gb.display.cursorY = 13;
  gb.display.setFont(font5x7);
  gb.display.print(F("YOU WIN!"));
  gb.display.setFont(font3x5);
  gb.display.cursorX = 18;
  gb.display.cursorY = 21;
  gb.display.print(F("\25:continue"));
  gb.display.cursorX = 18;
  gb.display.cursorY = 28;
  gb.display.print(F("\26:reset"));
}

void DrawLoseBox() {
  DrawBoard();
  gb.display.drawRect(13,15,57,18);
  gb.display.setColor(WHITE);
  gb.display.fillRect(14,16,55,16);
  gb.display.setColor(BLACK);
  gb.display.cursorX = 15;
  gb.display.cursorY = 17;
  gb.display.setFont(font5x7);
  gb.display.print(F("GAME OVER"));
  gb.display.setFont(font3x5);
  gb.display.cursorX = 27;
  gb.display.cursorY = 25;
  gb.display.print(F("Press \26"));
}
