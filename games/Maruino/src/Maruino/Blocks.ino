const byte *block;
char curLevel[384] = "";


void loadLevel(int curLev){
  for(int i = 0; i<384; i++){//Loops through the level
    curLevel[i] = (char)pgm_read_byte(lev + 384 * (curLev - 1) + i);
  }
  
  
}

void nextLevel(){
  if(curLev == numLevs){
    win();
  }
  else{
    gb.popup(F("Next Level!"), 20);
    if(curLev == 6){
      gb.popup(F("Remember lvl 1?"), 20);
    }
    curLev ++;
    loadLevel(curLev);
    player.y = player.x = 0;
    time = 130;
    cameraPos = 0;
  }
}

void drawLevel(){
  for(int i = 0; i<6; i++){
    for(int j = 0; j<64; j++){
      if((j*8 - cameraPos >= -8 && j*8 - cameraPos <= LCDWIDTH +8)){
        block = NULL;
        switch(curLevel[(i)*64+j]){
          case 'G' :
            block = Ground;
            break;
          case 'B' :
            block = Brick;
            break;
          case '?' :
            block = MysteryBlockC;
            break;
          case 'M' :
            block = MysteryBlockM;
            break;
          case 'C' :
            block = Coin;
            break;
          case 'S' :
            block = Spikes;
            break;
          case 'E' :
            block = Empty;
            break;
          case 'F' :
            block = Flag;
            break;
          case 'g' :
            curLevel[(i)*64+j] = 'Q';
            addEnt(j*8, LCDHEIGHT - (8 * (6-i)), Goomba);
            break;
          default:
            break;
        }
        
        if(block != NULL) {
          if((block == Coin || block == Spikes) && gb.frameCount%20 >= 10){
            gb.display.drawBitmap(j*8 - cameraPos, LCDHEIGHT - (8 * (6-i)), block, NOROT, FLIPH);
          }
          else{
            gb.display.drawBitmap(j*8 - cameraPos, LCDHEIGHT - (8 * (6-i)), block);
          }
        }
      }
      else if(curLevel[(i)*64+j] == 'Q'){
        curLevel[(i)*64+j] = 'g';
      }
      
    }
  }
}

