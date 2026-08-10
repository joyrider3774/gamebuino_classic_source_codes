//----------------------------------------------------------------------------
void newgame() {
  score=0;
  lives=3;
  gamelevel=0;
  ufotype=0;
  ufoshotx=-1;
  playershiprotation=0;
  bonusscore=10000;
  gamestatus="newlevel";
}
//----------------------------------------------------------------------------
void newlevel() {
  asterocksonscreen=4+gamelevel*2;
  if (asterocksonscreen>64) { asterocksonscreen=64; }
  for (int i=0; i<asterocksonscreen; i++) {
    int x=random(320);
    if (x>160) { x=x+448; }
    int y=random(128);
    asterockx[i]=x;
    asterocky[i]=y;
    asterocktype[i]=random(4);
    asterockxspeed[i]=rockxadd[random(4)]/4+8;
    asterockyspeed[i]=rockyadd[random(12)]/4+8;
  }
  playershipvisible=2;
  gamestatus="newlife";
  soundspeed=40;
  soundvalue=0;
}
//----------------------------------------------------------------------------
void newlife() {
  yeahtimer=0;
  deadtimer=-1;
  playershipx=320;
  playershipy=176;
  playershipxspeed=0;
  playershipyspeed=0;
  if (playershipvisible==1) { // after dead
    playershipvisible=0;
    playershiprotation=0;
  } else { // after level done
    playershipvisible=1;
  }
  gamestatus="running";
}
//----------------------------------------------------------------------------
void showscore() {
  i=1;
  while (lives>i) {
    gb.display.drawBitmap(i*6-6,0,playership[0]);
    i=++i;
  }
  gb.display.cursorX=40-2*(score>9)-2*(score>99)-2*(score>999)-2*(score>9999)-2*(score>99999);
  gb.display.cursorY=0;
  gb.display.print(score);
  gb.display.cursorX=76;
  gb.display.print(gamelevel+1);
}
//----------------------------------------------------------------------------
void nextlevelcheck() {
  if (yeahtimer>0) {
    yeahtimer=--yeahtimer;
    if (yeahtimer==0) {
      gamelevel=gamelevel+1;
      gamestatus="newlevel"; 
    } 
  }
}
//----------------------------------------------------------------------------
void handledeath() {
  deadtimer=--deadtimer;
  if (deadtimer % 5 == 0) {
    playsoundfx(1,0);
  }
  // draw dead animation
  i=19 - deadtimer / 10;
  gb.display.drawBitmap(playershipx/8,playershipy/8,playership[i]);  
  if (deadtimer==0) {
    deadtimer=-1;
    lives=--lives;  
    if (lives == 0) {
      gamestatus = "gameover";
    } else {
      gamestatus = "newlife";          
    }          
  }
}
