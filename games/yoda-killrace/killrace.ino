#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb;
//----------------------------------------------------------------------    
//                             K I L L R A C E
//                              by Yoda Zhang
//----------------------------------------------------------------------    

//----------------------------------------------------------------------------    
// define variables and constants
//----------------------------------------------------------------------------    
String gamestatus;
int score;
int highscore;
byte lives;
byte gamelevel;
byte carx;
byte cary;
int carxr;
int caryr;
byte carshape; // 0 = left, 1 = up, 2 = right, 3 = down, 4-7 = explo, +8 = crash
byte dustx[4];
byte dusty[4];
byte dustcounter;
byte manx[50];
byte many[50];
byte manshape[50]; // 0 - off screen, 1 - left, 2 - right, 3 - tombstone
int manxr[50];
byte manframe;
byte manframecounter;
byte manappearcounter;
byte mancounter;
byte mentokill;
byte menkilled;
byte manframemax;
int yeahtimer;
int deadcounter;
byte i;
//----------------------------------------------------------------------------    
// define images & sounds
//----------------------------------------------------------------------------    
extern const byte PROGMEM gamelogo[];
extern const byte PROGMEM background[];
extern const byte PROGMEM carsprite[12][10];
extern const byte PROGMEM mansprite[5][9];
extern const int soundfx[5][8];
//----------------------------------------------------------------------------    
// setup
//----------------------------------------------------------------------------    
void setup(){
  gb.begin();
  gb.setFrameRate(25);
  gb.titleScreen(F("    Yoda's"),gamelogo);
  gb.pickRandomSeed();
  gamestatus="title";
  gb.battery.show=false;
}

//----------------------------------------------------------------------------    
// loop
//----------------------------------------------------------------------------    
void loop(){
  if(gb.update()){

     // new game
    if (gamestatus=="newgame") { newgame(); }

    // new level
    if (gamestatus=="newlevel") { newlevel(); }
    
    // new life
    if (gamestatus=="newlife") { newlife(); }

     // game running
    if (gamestatus=="running") {
    
      checkbuttons(); // check buttons and determine car direction
      movecar(); // move and draw car
      manappear(); // man appears?
      movemen(); // move and draw men & tombstones
      checkcollission(); // check collission
      nextlevelcheck(); // next level?
      handledeath(); // handle deathcounter
      showscore(); // show lives, score, level
    } // end of: gamestatus = running
    
    // title
    if (gamestatus=="title") { showtitle(); }

    // game over
    if (gamestatus=="gameover") { showgameover(); }

  } // end of update
 } // end of loop
