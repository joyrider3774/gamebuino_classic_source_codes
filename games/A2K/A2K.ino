#include <Buttons.h>
#include <Display.h>
#include <Gamebuino.h>
#include <SPI.h>
#include <Sound.h>

/*  A to K, v.1.0.b
 Carlos Mari, Nov 2014, carlos@carloslabs.com
 
 A variant of the 2048 puzzle: move the board in the four directions to match letters of the same value and "promote them"
 up to the next letter up. Game is won when the player reaches "K"
 */

extern const byte font3x5[]; // default font
extern const byte font5x7[]; //  large, comfy font

const byte GRID_X = 4;
const byte GRID_Y = 4;
const byte POINTER_TEXT_X=8;
const byte POINTER_TEXT_Y=7;
const byte EMPTY_TILE=32;  // "blank space"
const byte A_TILE=65;      // 'A'
const byte K_TILE=75;      // 'K'
const byte F_COUNT=4;
const byte DELAY_MILLIS=50;

Gamebuino gb = Gamebuino();

byte grid_lines_x = 5;
byte grid_lines_y = 5;

byte grid_wide_x = 10;
byte grid_wide_y = 10;
byte grid_origin_x = 5;
byte grid_origin_y = 5;

byte emptyX, emptyY = 0;

word wScore, wMoves, freeMemory = 0;

boolean gridSet, bBusy, bReady, bGameOver, bGameWon=false;

byte GRID[GRID_X][GRID_Y];
word TILE_SCORE[12] = {
  2,4,8,16,32,64,128,256,512,1024,2048,4096};

// setup
void setup() {
  gb.begin(); 
  gb.display.setFont(font3x5);
  gb.titleScreen(F("\n>> A to K <<\n\nA 2048 clone\n\ncarloslabs.com"));
  gb.pickRandomSeed(); 
  gb.battery.show = false;
  gb.display.persistence = true;
  gb.display.clear();

  gridSet = initGrid();
  setRandomTile(2);
  //setRandomTile(14); // for testing
  renderGrid(); 
}

void loop() {

  if(gb.update()){   
    if(gb.buttons.pressed(BTN_C)){
      // change game
      gb.changeGame();
    }
    if(gb.buttons.pressed(BTN_A)){
      //todo UNDO ??
    }
    if(gb.buttons.pressed(BTN_B)){
      // reset game
      gb.display.clear();
      bGameOver = false;
      bGameWon = false;
      gridSet = initGrid();
      setRandomTile(2);   
      renderGrid();   
    }

    if((gb.buttons.pressed(BTN_UP))&&(!bBusy)&&(!bGameOver)){
      moveUp();
      wMoves++;    
    }
    if((gb.buttons.pressed(BTN_DOWN))&&(!bBusy)&&(!bGameOver)){
      moveDown();
      wMoves++;    
    }
    if((gb.buttons.pressed(BTN_LEFT))&&(!bBusy)&&(!bGameOver)){
      moveLeft();
      wMoves++;    
    }
    if((gb.buttons.pressed(BTN_RIGHT))&&(!bBusy)&&(!bGameOver)){
      moveRight();
      wMoves++;     
    }

    if ((bBusy)&&(!bGameOver))  {
      delay(DELAY_MILLIS);
      bBusy = false;
      setRandomTile(1);
      renderGrid();
    } 
    if (bGameOver)  {
      gameOver();
    }
    bGameOver = (!isEmptyTile());
    if (bGameWon)  {
      gameWon();
    }
  }
}

boolean initGrid() {  
  int g_x, grid_length = 0;

  gb.display.clear();
  wMoves = 0;
  wScore = 0;
  bBusy = false;

  for(byte y = 0; y < GRID_Y; y++){
    for(byte x = 0; x < GRID_X; x++){
      GRID[x][y] = EMPTY_TILE;
    }
  }

  // note: only calculating the X axis, as the 2048 (A to K) grid is square
  grid_length = grid_wide_x * (grid_lines_x -1);
  for(byte x = 0; x < grid_lines_x; x++){
    g_x = grid_origin_x + (x * grid_wide_x);
    gb.display.drawFastVLine(g_x, grid_origin_x, grid_length);
    gb.display.drawFastHLine(grid_origin_y, g_x, grid_length);
  }
  return true;
}

boolean isEmptyTile()  {
  for(byte y = 0; y < GRID_Y; y++){
    for(byte x = 0; x < GRID_X; x++){
      if(GRID[x][y] == EMPTY_TILE)
        return true;
    }
  }
  return false;  
}

void setRandomTile(byte numTiles)  {
  byte rand_x, rand_y, rand_z; 

  for(byte j = 0; j < (numTiles); j++){
    bReady = false;
    while (!bReady) {
      rand_x = random(4);
      rand_y = random(4);
      rand_z = random(2);
      if(GRID[rand_x][rand_y] == EMPTY_TILE){
        GRID[rand_x][rand_y] = (A_TILE + rand_z);
        bReady=true;
      }
    }
  }
}

void renderGrid()  {
  gb.display.setFont(font5x7);

  for(byte y = 0; y < GRID_Y; y++){
    for(byte x = 0; x < GRID_X; x++){
      gb.display.cursorX = POINTER_TEXT_X + (grid_wide_x * x);
      gb.display.cursorY = POINTER_TEXT_Y + (grid_wide_y * y);
      gb.display.print(char(GRID[x][y]));      
    }
  }
  renderData();
}

void renderData() {
  int cursorXpos = 0;

  gb.display.setFont(font5x7);
  gb.display.cursorX = 49;
  gb.display.cursorY = 1;
  gb.display.print("A to K");

  gb.display.setFont(font3x5);  
  gb.display.cursorX = 49;
  gb.display.cursorY = 14;        
  gb.display.print("[ Score ]");
  if (wScore >= 1000)  { 
    cursorXpos = 58; 
  }
  else if (wScore >= 100)  { 
    cursorXpos = 61; 
  }
  else { 
    cursorXpos = 64; 
  }
  gb.display.cursorX = cursorXpos;
  gb.display.cursorY = 22;        
  gb.display.print(wScore);         

  gb.display.cursorX = 49;
  gb.display.cursorY = 32;
  gb.display.print("[ Moves ]");
  if (wMoves >= 1000)  { 
    cursorXpos = 58; 
  }
  else if (wMoves >= 100)  { 
    cursorXpos = 61; 
  }
  else { 
    cursorXpos = 64; 
  }
  gb.display.cursorX = cursorXpos;  
  gb.display.cursorY = 40;        
  gb.display.print(wMoves);         
}

void addScore(byte bTile)  {
  int ix = (bTile-A_TILE);
  wScore+=TILE_SCORE[ix];
  if(bTile>=K_TILE)  { bGameWon=true; }
}

void gameWon() {
  gb.display.setColor(BLACK);
  gb.display.fillRect(2, 5, 55, 32);  
  gb.display.setColor(WHITE);
  gb.display.fillRect(3, 6, 53, 30);
  gb.display.setColor(BLACK); 
  gb.display.cursorX = 7;
  gb.display.cursorY = 9;
  gb.display.print("YOU WON A2K!\n\n   Press B to \n    restart"); 
}

void gameOver() {
  gb.display.setColor(BLACK);
  gb.display.fillRect(2, 5, 55, 32);  
  gb.display.setColor(WHITE);
  gb.display.fillRect(3, 6, 53, 30);
  gb.display.setColor(BLACK); 
  gb.display.cursorX = 12;
  gb.display.cursorY = 9;
  gb.display.print("GAME OVER\n\n   Press B to \n    restart"); 
}

void moveLeft()  {
  byte fCount = 0;
  bBusy = true; 
  while (fCount < F_COUNT){
    fCount++;
    for(byte y = 0; y < GRID_Y; y++){
      if(GRID[0][y]==EMPTY_TILE){
        GRID[0][y]=GRID[1][y];
        GRID[1][y]=EMPTY_TILE;
      }
      if((GRID[0][y]==GRID[1][y])&&(GRID[0][y] > EMPTY_TILE)){
        // two equal tiles, promote
        GRID[0][y]=1+(GRID[0][y]);
        addScore(GRID[0][y]);
        GRID[1][y]=EMPTY_TILE;          
      }
      if((GRID[0][y]!=GRID[1][y])&&(GRID[0][y] > EMPTY_TILE)&&(GRID[1][y] > EMPTY_TILE)){
        // two different tiles, adjacent
      }               
      if(GRID[1][y]==EMPTY_TILE){
        GRID[1][y]=GRID[2][y];
        GRID[2][y]=EMPTY_TILE;
      }
      if((GRID[1][y]==GRID[2][y])&&(GRID[1][y] > EMPTY_TILE)){
        GRID[1][y]=1 + GRID[2][y];
        addScore(GRID[1][y]);        
        GRID[2][y]=EMPTY_TILE;      
      }
      if((GRID[1][y]!=GRID[2][y])&&(GRID[1][y] > EMPTY_TILE)&&(GRID[2][y] > EMPTY_TILE)){
        // two different tiles, adjacent
      }          
      if(GRID[2][y]==EMPTY_TILE){
        GRID[2][y]=GRID[3][y];
        GRID[3][y]=EMPTY_TILE;
      }
      if((GRID[2][y]==GRID[3][y])&&(GRID[2][y] > EMPTY_TILE)){
        GRID[2][y]=1 + GRID[3][y];
        addScore(GRID[2][y]);          
        GRID[3][y]=EMPTY_TILE;           
      }
      if((GRID[2][y]!=GRID[3][y])&&(GRID[2][y] > EMPTY_TILE)&&(GRID[3][y] > EMPTY_TILE)){
        // two different tiles, adjacent
      }
      if(GRID[3][y]==EMPTY_TILE){
        // nothing, we are moving left
      }         
    }
  }
}

void moveRight()  {
  byte fCount = 0;
  bBusy = true;  
  while (fCount < F_COUNT){
    fCount++;
    for(byte y = 0; y < GRID_Y; y++){                     
      if((GRID[2][y]!=GRID[3][y])&&(GRID[2][y] > EMPTY_TILE)&&(GRID[3][y] > EMPTY_TILE)){
        // two different tiles, adjacent
      }
      if((GRID[2][y]==GRID[3][y])&&(GRID[3][y] > EMPTY_TILE)){
        GRID[3][y]=1 + GRID[3][y];
        addScore(GRID[3][y]);
        GRID[2][y]=EMPTY_TILE;
      }
      if(GRID[3][y]==EMPTY_TILE){
        GRID[3][y]=GRID[2][y];
        GRID[2][y]=EMPTY_TILE;
      }
      if((GRID[1][y]!=GRID[2][y])&&(GRID[1][y] > EMPTY_TILE)&&(GRID[2][y] > EMPTY_TILE)){
        // two different tiles, adjacent
      } 
      if((GRID[1][y]==GRID[2][y])&&(GRID[2][y] > EMPTY_TILE)){
        GRID[2][y]=1 + GRID[2][y];
        addScore(GRID[2][y]);
        GRID[1][y]=EMPTY_TILE;
      }
      if(GRID[2][y]==EMPTY_TILE){
        GRID[2][y]=GRID[1][y];
        GRID[1][y]=EMPTY_TILE;
      }
      if((GRID[0][y]!=GRID[1][y])&&(GRID[0][y] > EMPTY_TILE)&&(GRID[1][y] > EMPTY_TILE)){
        // two different tiles, adjacent
      }
      if((GRID[0][y]==GRID[1][y])&&(GRID[1][y] > EMPTY_TILE)){
        GRID[1][y]=1+(GRID[0][y]);
        addScore(GRID[1][y]);
        GRID[0][y]=EMPTY_TILE;
      }
      if(GRID[1][y]==EMPTY_TILE){
        GRID[1][y]=GRID[0][y];
        GRID[0][y]=EMPTY_TILE;
      }      
      if(GRID[0][y]==EMPTY_TILE){
        // nothing, we are moving right
      }
      
    }
  }  
}

void moveDown()  {
  byte fCount = 0;
  bBusy = true;
  while (fCount < F_COUNT){
    fCount++;
    for(byte x = 0; x < GRID_X; x++){

      if((GRID[x][2]!=GRID[x][3])&&(GRID[x][2] > EMPTY_TILE)&&(GRID[x][3] > EMPTY_TILE)){
        // two different tiles, adjacent
      }        
      if((GRID[x][2]==GRID[x][3])&&(GRID[x][3] > EMPTY_TILE)){
        GRID[x][3]=1 + GRID[x][3];
        addScore(GRID[x][3]);
        GRID[x][2]=EMPTY_TILE;
      }
      if(GRID[x][3]==EMPTY_TILE){
        GRID[x][3]=GRID[x][2];
        GRID[x][2]=EMPTY_TILE;
      }
      if((GRID[x][1]!=GRID[x][2])&&(GRID[x][1] > EMPTY_TILE)&&(GRID[x][2] > EMPTY_TILE)){
        // two different tiles, adjacent
      } 
      if((GRID[x][1]==GRID[x][2])&&(GRID[x][2] > EMPTY_TILE)){
        GRID[x][2]=1 + GRID[x][2];
        addScore(GRID[x][2]);
        GRID[x][1]=EMPTY_TILE;
      }
      if(GRID[x][2]==EMPTY_TILE){
        GRID[x][2]=GRID[x][1];
        GRID[x][1]=EMPTY_TILE;
      }
      if((GRID[x][0]!=GRID[x][1])&&(GRID[x][0] > EMPTY_TILE)&&(GRID[x][1] > EMPTY_TILE)){
        // two different tiles, adjacent
      }
      if((GRID[x][0]==GRID[x][1])&&(GRID[x][1] > EMPTY_TILE)){
        GRID[x][1]=1+(GRID[x][0]);
        addScore(GRID[x][1]);
        GRID[x][0]=EMPTY_TILE;
      }   
      if(GRID[x][1]==EMPTY_TILE){
        GRID[x][1]=GRID[x][0];
        GRID[x][0]=EMPTY_TILE;
      }      
      if(GRID[x][0]==EMPTY_TILE){
        // nothing, we are moving down
      }
      
    }
  }
}

void moveUp()  {
  byte fCount = 0;
  bBusy = true;  
  while (fCount < F_COUNT){
    fCount++;
    for(byte x = 0; x < GRID_X; x++){
      if(GRID[x][0]==EMPTY_TILE){
        GRID[x][0]=GRID[x][1];
        GRID[x][1]=EMPTY_TILE;
      }
      if((GRID[x][0]==GRID[x][1])&&(GRID[x][0] > EMPTY_TILE)){
        GRID[x][0]=1+(GRID[x][0]);
        addScore(GRID[x][0]);
        GRID[x][1]=EMPTY_TILE;
      }
      if((GRID[x][0]!=GRID[x][1])&&(GRID[x][0] > EMPTY_TILE)&&(GRID[x][1] > EMPTY_TILE)){
        // two different tiles, adjacent
      }               
      if(GRID[x][1]==EMPTY_TILE){
        GRID[x][1]=GRID[x][2];
        GRID[x][2]=EMPTY_TILE;
      }
      if((GRID[x][1]==GRID[x][2])&&(GRID[x][1] > EMPTY_TILE)){
        GRID[x][1]=1 + GRID[x][2];
        addScore(GRID[x][1]);
        GRID[x][2]=EMPTY_TILE;
      }
      if((GRID[x][1]!=GRID[x][2])&&(GRID[x][1] > EMPTY_TILE)&&(GRID[x][2] > EMPTY_TILE)){
        // two different tiles, adjacent
      }          
      if(GRID[x][2]==EMPTY_TILE){
        GRID[x][2]=GRID[x][3];
        GRID[x][3]=EMPTY_TILE;
      }
      if((GRID[x][2]==GRID[x][3])&&(GRID[x][2] > EMPTY_TILE)){
        GRID[x][2]=1 + GRID[x][3];
        addScore(GRID[x][2]);
        GRID[x][3]=EMPTY_TILE;
      }
      if((GRID[x][2]!=GRID[x][3])&&(GRID[x][2] > EMPTY_TILE)&&(GRID[x][3] > EMPTY_TILE)){
        // two different tiles, adjacent
      }
      if(GRID[x][3]==EMPTY_TILE){
        // nothing, we are moving up
      }         
    }
  }
}







