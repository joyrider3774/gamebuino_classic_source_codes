
void initMenu(byte lvl){
  calc_unlockedLevel();
  current_num_level = lvl;//Chargement du bon niveau
  game_complete = 1;
  for(byte i = 0; i < NOMBRE_NIVEAUX ; i++){
    if(complete[i] != 1){
      game_complete = 0;
    }  
    EEPROM.write(i, complete[i]); //LSB
    EEPROM.write(32+i*2+0, highscores[i] & 0x00FF); //LSB
    EEPROM.write(32+i*2+1, (highscores[i] >> 8) & 0x00FF); //MSB
  }
}

void updateMenu(){

  nom_niveau();
  menu_frames++;
  if(menu_frames >= 160) menu_frames = 32;
  if(menu_frames >= 2 && menu_frames < 32) menu_frames = 32;

  if(gb.buttons.pressed(BTN_C) || gb.buttons.pressed(BTN_B)){
    gb.sound.playCancel();
    initGame();
  }

  //Dessin des cercles de niveau
  char drawX, drawY, draw_oldX = 0, draw_oldY = 0;
  for(byte i = 0; i < 6; i++){
    char niveau_dessine = current_num_level + i - 2;
    drawX = 20*i-10; 
    drawY = 20+5*(niveau_dessine % 2);
    if(niveau_dessine < 0) drawY = 27;
    if(niveau_dessine >= -2 && niveau_dessine <= unlocked_level && niveau_dessine < NOMBRE_NIVEAUX){ 
      if(niveau_dessine >= -1 && draw_oldY != 0) {
        gb.display.drawLine(draw_oldX+7, draw_oldY+4, drawX+4, drawY+4);
        gb.display.setColor(WHITE);
        gb.display.fillRect(drawX+1,drawY+1,6,6);
        gb.display.setColor(BLACK);
      }
      if(niveau_dessine >= 0){
        if(highscores[niveau_dessine] > target[niveau_dessine] || highscores[niveau_dessine] <= 20 || highscores[niveau_dessine] >= 2000){
          if(i == 2 && menu_frames % 16 < 10) gb.display.drawBitmap(drawX,drawY,level_icon_2);
          else gb.display.drawBitmap(drawX,drawY,level_icon_1);
        }
        else{
          if(i == 2 && menu_frames % 16 < 10) gb.display.drawBitmap(drawX,drawY,level_icon_4);
          else gb.display.drawBitmap(drawX,drawY,level_icon_3);
        }
      }
      if(niveau_dessine == -1){
        if(i == 2 && menu_frames % 16 < 10) gb.display.drawBitmap(drawX,drawY,level_icon_8);
        else gb.display.drawBitmap(drawX,drawY,level_icon_7);
      }
      if(niveau_dessine == -2){
        if(i == 2 && menu_frames % 16 < 10) gb.display.drawBitmap(drawX,drawY,level_icon_6);
        else gb.display.drawBitmap(drawX,drawY,level_icon_5);
      }
    }
    draw_oldX = drawX; 
    draw_oldY = drawY;

    if(i == 2 && niveau_dessine >= 0 && menu_frames >= 32){
      gb.display.cursorX = 0;
      gb.display.cursorY = 6;
      if(complete[niveau_dessine] == 0 || highscores[niveau_dessine] <= 20){
        gb.display.print(F("Go for it !"));
      }
      else{
        if(menu_frames % 64 >= 32){
          gb.display.print(F("Your record:"));
          afficheTimer(highscores[niveau_dessine], 64, 6);
        }
        else{
          gb.display.print(F("Target time:"));
          afficheTimer(target[niveau_dessine], 64, 6);
        }
      }
    }
  }
  
  if(game_complete == 1){
      gb.display.cursorX = 4;
      gb.display.cursorY = LCDHEIGHT-6;
      gb.display.print(F("All levels complete!"));
  }




  if(gb.buttons.pressed(BTN_LEFT) && current_num_level > -2){ 
    current_num_level--; 
    menu_frames = 32;
    gb.sound.playTick();
  }
  if(gb.buttons.pressed(BTN_RIGHT) && current_num_level < unlocked_level && current_num_level < NOMBRE_NIVEAUX-1){ 
    current_num_level++; 
    menu_frames = 32;
    gb.sound.playTick();
  }
  current_num_level = min(current_num_level, NOMBRE_NIVEAUX-1);
  if(gb.buttons.pressed(BTN_A) && current_num_level >= 0 && menu_frames > 5){
    mode = NIVEAU;
    joueur.angle_laststart = 10;
    initNiveau(current_num_level); 
    gb.sound.playOK();
  }
  if(gb.buttons.pressed(BTN_A) && current_num_level < 0 && menu_frames > 5){
    mode = CREDITS;
    gb.sound.playOK();
  }
}

void calc_unlockedLevel(){//Recalcule le plus grand niveau débloqué
  for(byte i = max(1,unlocked_level); i < NOMBRE_NIVEAUX; i++){
    if(complete[i-1] == 1){
      unlocked_level = i; 
    }
  }
}

void nom_niveau(){
  gb.display.cursorX = 0;
  gb.display.cursorY = 0;
  switch(current_num_level){
  case -2:
    {
      gb.display.print(F("Unlock/Erase records"));
    } 
    break;
  case -1:
    {
      gb.display.print(F("Credits"));
    } 
    break;
  case 0:
    {
      gb.display.print(F("1A: Start spinning!"));
    } 
    break;
  case 1:
    {
      gb.display.print(F("1B: Zig Zag"));
    } 
    break;
  case 2:
    {
      gb.display.print(F("1C: Narrow passages"));
    } 
    break;
  case 3:
    {
      gb.display.print(F("1D: Hurdles"));
    } 
    break;
  case 4:
    {
      gb.display.print(F("2A: Springs"));
    } 
    break;
  case 5:
    {
      gb.display.print(F("2B: Boing Boing"));
    } 
    break;
  case 6:
    {
      gb.display.print(F("2C: Right or left?"));
    } 
    break;
  case 7:
    {
      gb.display.print(F("2D: Patience"));
    } 
    break;

  }

}










