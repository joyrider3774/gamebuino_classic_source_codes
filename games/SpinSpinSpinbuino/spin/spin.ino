#include <SPI.h>
#include <EEPROM.h>
#include <Gamebuino.h>
Gamebuino gb;
#define TAILLE_JOUEUR 2
#define TAILLE_BATON 10
#define JOUEUR_ECRAN_X 42
#define JOUEUR_ECRAN_Y 24
#define VITESSE_DROITE 10
#define VITESSE_DIAG 7
#define NOMBRE_NIVEAUX 8

const unsigned int destroy[] PROGMEM = {
  0x8045,0x8891,0x8241,0x608,0x0000};


typedef struct {
  byte x, y;
  char x_diff, y_diff;
  int x_full, y_full;
  byte angle;
  byte angle_laststart;
  byte hit;
  boolean reverse;
  char taille;
  byte circle_x, circle_y;
  unsigned int timer, highscore;
  byte x_goal, y_goal;
  byte goal;
  byte max_ressorts;
  boolean start;
  char derniere_direction;
  boolean pause;
  boolean pause_off;

} 
Joueur;

Joueur joueur;
prog_uchar* current_level;
prog_uchar* current_settings;

byte animation_ressorts[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

enum Mode {
  MENU, NIVEAU, CREDITS };
Mode mode;
char current_num_level = 0;
boolean complete[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 
byte unlocked_level = 0;
unsigned int highscores[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 
unsigned int target[] = {
  140,140,160,280,240,220,160,300,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 
byte menu_frames = 0;
boolean game_complete = 0;

// the setup routine runs once when Gamebuino starts up
void setup(){
  gb.begin();
  initGame();
}

void loop(){
  if(gb.update()){

    switch(mode){
    case NIVEAU :
      {
        if(!joueur.pause) updateJoueur();
        afficheDecor();
        if (!joueur.pause && joueur.hit == 0 && joueur.goal == 0) collisionsJoueur(0);
        afficheRessorts();
        if (!joueur.pause && joueur.hit == 0 && joueur.goal == 0) collisionsJoueur(1);
        afficheArrivee();
        if(joueur.taille >= 0) afficheJoueur();
        if(joueur.goal > 20) afficheFin();
        if(joueur.taille < 6) afficheRetry();
        afficheTimer(joueur.timer, 64, 0);

        //GESTION DE LA PAUSE
        if(joueur.pause){
          affichePause();
          if(gb.buttons.pressed(BTN_C)){
            joueur.pause = 0; 
            joueur.pause_off = 1;
            gb.sound.playOK();
          }
          if(gb.buttons.pressed(BTN_B)){
            initNiveau(current_num_level);
            gb.sound.playCancel();
          }
          if(gb.buttons.pressed(BTN_A)){
            mode = MENU;
            menu_frames = 0;
            initMenu(current_num_level);
            gb.sound.playCancel();
          }
        }
        if(joueur.hit == 0 && joueur.goal == 0 && !joueur.pause && !joueur.pause_off && gb.buttons.pressed(BTN_C)){
          joueur.pause = 1; 
          gb.sound.playCancel();
        }
        joueur.pause_off = 0;

        //GESTION DE LA FIN DE PARTIE
        if(joueur.goal > 20){
          complete[current_num_level] = 1;
          if(joueur.timer < highscores[current_num_level] || joueur.highscore <= 20 || joueur.highscore >= 2000) highscores[current_num_level] = joueur.timer;
          if(gb.buttons.pressed(BTN_B)){
            initNiveau(current_num_level);
            gb.sound.playCancel();
          }
          if((gb.buttons.pressed(BTN_A) && current_num_level >= NOMBRE_NIVEAUX-1) || gb.buttons.pressed(BTN_C)){
            mode = MENU;
            menu_frames = 0;
            initMenu(current_num_level);
            gb.sound.playCancel();
          }
          if(gb.buttons.pressed(BTN_A) && current_num_level < NOMBRE_NIVEAUX-1){
            initNiveau(current_num_level+1);
            gb.sound.playOK();
          }
        }


        /*       gb.display.setColor(WHITE);
         gb.display.fillRect(0, 0, 8, 6);
         gb.display.setColor(BLACK);
         gb.display.cursorX = 0;
         gb.display.cursorY = 0;
         //gb.display.print(gb.getCpuLoad());
         gb.display.print(joueur.angle_laststart);*/

      }
      break;
    case MENU :
      {
        updateMenu();
      }
      break;
    default :
      {
        updateCredits();
      }
      break;
    }


  }
}

void initGame(){
  gb.titleScreen(F("Spin Spin Spinbuino!"));
  gb.battery.show = false;
  mode = MENU;
  menu_frames = 0;
  for(byte i = 0; i < NOMBRE_NIVEAUX; i++){
    complete[i] = EEPROM.read(i); 
    highscores[i] = EEPROM.read(32+i*2+0) & 0x00FF; //LSB
    highscores[i] += (EEPROM.read(32+i*2+1) << 8) & 0xFF00; //MSB
    if(highscores[i] > 2000) highscores[i] = 0;

  }
  initMenu(unlocked_level);
  //initNiveau(2);
}









