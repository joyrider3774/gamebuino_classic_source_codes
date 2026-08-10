#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb = Gamebuino();


#define MENULENGTH 4
const char strPlay[] PROGMEM = "Play Game";
const char strMainMenu[] PROGMEM = "Main Menu";
const char strCode[] PROGMEM = "Input a code";
const char strControls[] PROGMEM = "Controls";
const char* const menu[MENULENGTH] PROGMEM = {
  strPlay, strMainMenu, strControls, strCode,
};


typedef struct {
  int invincible;
  float x;
  float vx;
  float y;
  float vy;
  byte h;
  boolean flip;
  boolean big;
  boolean crouch;
} Char;

typedef struct{
  float x;
  float y;
  float vx;
  float vy;
  boolean flip;
  const byte *bitmap;
} Entity;


extern const byte font3x3[];
extern const byte font3x5[];
extern const byte font5x7[];
extern const byte Title[] PROGMEM;
extern const char lev[] PROGMEM;
extern const byte Ground[] PROGMEM;
extern const byte Spikes[] PROGMEM;
extern const byte Brick[] PROGMEM;
extern const byte MysteryBlockC[] PROGMEM;
extern const byte MysteryBlockM[] PROGMEM;
extern const byte Coin[] PROGMEM;
extern const byte Empty[] PROGMEM;
extern const byte PlayerS[] PROGMEM;
extern const byte PlayerS1[] PROGMEM;
extern const byte PlayerT[] PROGMEM;
extern const byte PlayerT1[] PROGMEM;
extern const byte PlayerT2[] PROGMEM;
extern const byte PlayerT3[] PROGMEM;
extern const byte PlayerT4[] PROGMEM;
extern const byte PlayerT5[] PROGMEM;
extern const byte PlayerT6[] PROGMEM;
extern const byte PlayerC[] PROGMEM;
extern const byte Mushroom[] PROGMEM;
extern const byte Goomba[] PROGMEM;
extern const byte Flag[] PROGMEM;
Char player;


int cameraPos;
int frames;
int score;
int curLev;
int numLevs = 6;
int lives = 5;
byte time;
boolean running;
boolean disp = true;
char code[12] = "";




void setup(){
  // initialize the Gamebuino object
  gb.begin();
  gb.display.setFont(font5x7);
  gb.battery.show = false;
  start();
}


void loop(){
  if(gb.update()){
    gb.battery.show = false;
    if(running){
      updatePlayer();
      updateEnt();
      drawPlayer();
      drawLevel();
      updateDisplay();
    }
    
  } 
}

void Menu(){
  running = false;
  gb.display.setFont(font5x7);
  switch(gb.menu(menu, MENULENGTH)){
    case -1: //Nothing
      start();
      break;
    case 0: //Play game
      startGame();
      break;
    case 1: //Main menu
      start();
      break;
    case 2: //Display controls
      controls();
      break;
    case 3: //Code input screen
      inputCode();
      break;
    default:
      break;
  }
}

void start(){
  // show the start menu
  gb.titleScreen(F("By: ajsb113"), Title);
  Menu();
  
}
void startGame(){
  cameraPos = 0;
  score = 0;
  lives = 5;
  running = true;
  curLev = 1;
  clearEnts();
  startTimer();
  startPlayer();
  loadLevel(curLev);
  
}

void inputCode(){
  while(!gb.buttons.pressed(BTN_A)){
    if(gb.update()){
      gb.display.cursorX = 0;
      gb.display.cursorY = 5;
      gb.display.textWrap = true;
      gb.display.println("Press \25 to    enter a code  or \27 to return to menu");
      gb.display.textWrap = false;
    }
  }
  gb.keyboard(code, 12);
  Menu();
}

void controls(){
  while(!gb.buttons.pressed(BTN_A)){
    if(gb.update()){
      gb.display.cursorX = 0;
      gb.display.cursorY = 0;
      gb.display.setFont(font3x5);
      gb.display.textWrap = true;
      gb.display.println("Use \33 & \32 to move.");
      gb.display.cursorY += 1;
      gb.display.println("Press \30 to toggle HUD size.");
      gb.display.cursorY += 1;
      gb.display.println("Press \25 to   jump.");
      gb.display.cursorY += 1;
      gb.display.println("Hold \26 to crouch");
      gb.display.cursorY += 1;
      gb.display.println("Press \27 to return to menu.");
    }
  }
  Menu();
}

void lose(){//temporary
  running = false;
  while(!gb.buttons.pressed(BTN_A)){
    if(gb.update()){
      gb.display.setFont(font5x7);
      gb.display.cursorY = 5;
      gb.display.cursorX = 15;
      
      gb.display.println("You lose \1");
      gb.display.print("Score:");
      gb.display.println(score);
      gb.display.println("Press \25 to \n continue");
    }
  }
 Menu();
}

void win(){
  running = false;
  while(!gb.buttons.pressed(BTN_A)){
    if(gb.update()){
      gb.display.setFont(font5x7);
      gb.display.cursorY = 5;
      gb.display.cursorX = 20;
      
      gb.display.println("You win!!\2");
      gb.display.print("Score:");
      gb.display.println(score);
      gb.display.println("Press \25 to \n continue");
    }
  }
 Menu();
}

void startTimer(){
  time = 130; 
  frames = gb.frameCount;
}

void updateDisplay(){
  if((gb.frameCount - frames)>=20){
    frames = gb.frameCount;
    time --;
  }
  if(disp){
    gb.display.fillRect(0,0,86,7);
    gb.display.setColor(WHITE);
    gb.display.setFont(font3x5);
    gb.display.cursorY = 1;
    gb.display.cursorX = 1;
    gb.display.print("\35:");
    gb.display.print(time);
    gb.display.print("  \44:");
    gb.display.print(score);
    gb.display.print("  \03:");
    gb.display.print(lives);
    gb.display.setColor(BLACK);
  }
  else{
    gb.display.fillRect(0,0,86,5);
    gb.display.setColor(WHITE);
    gb.display.setFont(font3x3);
    gb.display.cursorY = 1;
    gb.display.cursorX = 1;
    gb.display.print("\35:");
    gb.display.print(time);
    gb.display.print("  \44:");
    gb.display.print(score);
    gb.display.print("  \03:");
    gb.display.print(lives);
    gb.display.setColor(BLACK);
  }
  gb.display.setFont(font3x5);
}

