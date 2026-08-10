//----------------------------------------------------------------------------
void newgame() {
  score=0;
  lives=3;
  gamelevel=0;
  gamestatus="newlevel";
}
//----------------------------------------------------------------------------
void newlevel() {
  for (mancounter = 0; mancounter<50 ; mancounter++) {
    manshape[mancounter]=10;
  }
  mentokill = 6 + gamelevel*2;
  menkilled=0;
  manappearcounter=20;
  yeahtimer=0;
  mancounter=0;
  manframemax=6-gamelevel;
  if (manframemax<1) { manframemax = 1; }
  gamestatus="newlife";
}
//----------------------------------------------------------------------------
void newlife() {
  carx=38;
  cary=22;
  carxr=0;
  caryr=0;
  carshape=1;
  deadcounter=-1;
  for (i=0; i<4; i++) {
    dustx[i]=100;
    dusty[i]=0;
  }
  gamestatus="running";    
}
//----------------------------------------------------------------------------
void showscore() {
  if (cary>4) {
    gb.display.cursorY=0;
    gb.display.cursorX=40-2*(score>9)-2*(score>99)-2*(score>999)-2*(score>9999);
    gb.display.print(score);
  }
}
//----------------------------------------------------------------------------
void nextlevelcheck() {
  // increment timer after all men killed
  if (mentokill==menkilled) {
    yeahtimer=++yeahtimer;
    gb.display.setColor(0);
    gb.display.fillRect(5,17,74,14);
    gb.display.setColor(1);
    gb.display.cursorX=6;
    gb.display.cursorY=18;
    gb.display.print("READY FOR LEVEL");
    gb.display.cursorX=70;
    gb.display.print(gamelevel+2);
    gb.display.cursorX=26;
    gb.display.cursorY=24;
    gb.display.print("KILL");
    gb.display.cursorX=48;
    gb.display.print(mentokill+2);
    if (yeahtimer>=50) {
      gamelevel=++gamelevel;
       gamestatus="newlevel";
    }          
  }
}
//----------------------------------------------------------------------------
void handledeath() {
  if (deadcounter != -1) {
    deadcounter=--deadcounter;
    gb.display.setColor(0);
    gb.display.fillRect(19,19,46,8);
    gb.display.setColor(1);
    gb.display.cursorX=20;
    gb.display.cursorY=20;
    gb.display.print(lives-1);
    gb.display.cursorX=28;
    gb.display.print("CARS LEFT");
    if (deadcounter == 0) {
      deadcounter=-1;
      lives=--lives;  
      if (lives == 0) {
        gamestatus = "gameover";
      } else {
        gamestatus = "newlife";
      }      
    }
  }
}
