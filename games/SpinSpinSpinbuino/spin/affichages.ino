void afficheFin(){
  if(joueur.highscore > 20 && joueur.highscore < 2000){

    gb.display.setColor(WHITE);
    gb.display.fillRect(0, 0, LCDWIDTH, 12);
    gb.display.setColor(BLACK);
    afficheTimer(joueur.highscore, 64, 6);
    gb.display.cursorX = 0;
    if(joueur.timer < joueur.highscore){
      gb.display.cursorY = 0;
      gb.display.print(F("New record!"));
      gb.display.cursorX = 0;
      gb.display.cursorY = 6;
      gb.display.print(F("Previous record:"));
    }
    else{
      gb.display.cursorY = 0;
      gb.display.print(F("Time:"));
      gb.display.cursorX = 0;
      gb.display.cursorY = 6;
      gb.display.print(F("Your record:"));
    }
  }
  else{
    gb.display.setColor(WHITE);
    gb.display.fillRect(0, 0, LCDWIDTH, 6);
    gb.display.setColor(BLACK);
    gb.display.cursorX = 0;
    gb.display.cursorY = 0;
    gb.display.print(F("Time:"));
  }
    gb.display.setColor(WHITE);
    gb.display.fillRect(0, LCDHEIGHT-6, LCDWIDTH, 6);
    gb.display.setColor(BLACK);
    gb.display.cursorX = 6;
    gb.display.cursorY = LCDHEIGHT-6;
    gb.display.print(F("\25Next \26Retry \27Menu"));
}

void afficheRetry(){
    gb.display.setColor(WHITE);
    gb.display.fillRect(0, LCDHEIGHT-6, LCDWIDTH, 6);
    gb.display.setColor(BLACK);
    gb.display.cursorX = 14;
    gb.display.cursorY = LCDHEIGHT-6;
    gb.display.print(F("\25/\26Retry \27Menu"));
}

void affichePause(){
    gb.display.setColor(WHITE);
    gb.display.fillRect(0, LCDHEIGHT-6, LCDWIDTH, 6);
    gb.display.setColor(BLACK);
    gb.display.cursorX = 0;
    gb.display.cursorY = LCDHEIGHT-6;
    gb.display.print(F("\25Menu \26Retry \27Unpause"));
}


void afficheTimer(int frames, byte posX, byte posY){
   gb.display.setColor(WHITE);
    gb.display.fillRect(posX-1, posY, 21, 6);
    gb.display.setColor(BLACK);
    gb.display.cursorY = posY;
    gb.display.cursorX = posX;
  if(frames >= 2000){
    
    
    gb.display.print("99\"99");
  }
  else{
    gb.display.print(frames/200%10);
    gb.display.cursorX = posX+16;
    gb.display.print((frames % 2)*5);
    gb.display.cursorX = posX+12;
    gb.display.print(frames*5/10%10);
    gb.display.cursorX = posX+8;
    gb.display.print("\"");
    gb.display.cursorX = posX+4;
    gb.display.print(frames/20%10);

  }

}
