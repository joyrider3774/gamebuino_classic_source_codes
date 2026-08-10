#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;
//----------------------------------------------------------------------    
//                            A S T E R O C K S
//                              by Yoda Zhang
//----------------------------------------------------------------------    
// Please use this source-code for learning purposes only. If you make
// changes to the code, it may not compile to the same result as the
// provided HEX file, since I made changes to my compiler settings.
//----------------------------------------------------------------------    


// define variables and constants
String gamestatus;
int score;
int highscore;
byte lives;
byte gamelevel;
int yeahtimer;
int deadtimer;
int i;
int u;
float x;
float y;

int playershipx;
int playershipy;
int playershiprotation;
int playershipxspeed;
int playershipyspeed;
byte playershipvisible;
byte playershipappear;
int xadd[16]={0,4,8,8,8,8,8,4,0,-4,-8,-8,-8,-8,-8,-4};
int yadd[16]={-8,-8,-8,-4,0,4,8,8,8,8,8,4,0,-4,-8,-8};
int rockxadd[12]={4,4,-4,-4,8,8,8,8,-8,-8,-8,-8};
int rockyadd[12]={-8,8,-8,8,-8,-4,4,8,8,4,-4,-8};
int playershotx[4];
int playershoty[4];
int playershotxspeed[4];
int playershotyspeed[4];
byte playershotcounter[4];
byte playershots;
int bonusscore;
byte soundspeed;
byte soundvalue;
byte soundcounter;

int asterockx[32];
int asterocky[32];
byte asterockxspeed[32];
byte asterockyspeed[32];
byte asterocktype[32];
byte asterocksonscreen;
byte destroyed;
byte left;
byte right;

int ufox;
int ufoy;
byte ufotype;
int ufoxr;
int ufoyr;
int ufoshotx;
int ufoshoty;
int ufoshotxr;
int ufoshotyr;

// define images & sounds
extern const byte PROGMEM gamelogo[];
extern const byte PROGMEM playership[20][9];
extern const byte PROGMEM asterocks[12][22];
extern const byte PROGMEM ufo[2][7];
extern const byte PROGMEM bullet[];
extern const int soundfx[10][8];

// setup
void setup(){
  gb.begin();
  gb.setFrameRate(25);
  gb.titleScreen(F("    Yoda's"),gamelogo);
  gb.pickRandomSeed();
  gamestatus="title";
  gb.battery.show=false;
}

// loop
void loop(){
  if(gb.update()){

    if (gamestatus=="newgame") { newgame(); } // new game

    if (gamestatus=="newlevel") { newlevel(); } // new level

    if (gamestatus=="newlife") { newlife(); } // new life

    if (gamestatus=="running"){ // game running
      handleplayership(); // move and draw playership, check buttons etc.      
      handleplayershots(); // handle playershots
      handlerocks(); // handle asterocks
      nextlevelcheck(); // level finished?
      ufoappears(); // ufo appears?
      moveufo(); // move ufo
      playershotufocollission(); // check collission playershot & ufo
      ufoshotrelease();  // ufo shot release
      moveufoshot(); // move ufoshot
      checkbonuslife(); // bonuslife?
      showscore();  // show lives, score, level
      backgroundsound(); // make background sounds
    }

    if (gamestatus=="title") { showtitle(); } // title

    if (gamestatus=="gameover") { showgameover(); } // game over

  } // end of gb.update
 } // end of loop
