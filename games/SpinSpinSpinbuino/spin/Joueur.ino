void afficheJoueur(){
  gb.display.fillCircle(JOUEUR_ECRAN_X, JOUEUR_ECRAN_Y, TAILLE_JOUEUR*joueur.taille/TAILLE_BATON);
  float angle_rad = joueur.angle*0.0174;
  float x0 = JOUEUR_ECRAN_X+cos((joueur.angle+90)*0.0174);
  float y0 = JOUEUR_ECRAN_Y+sin((joueur.angle+90)*0.0174);
  float x1 = JOUEUR_ECRAN_X+cos((joueur.angle-90)*0.0174);
  float y1 = JOUEUR_ECRAN_Y+sin((joueur.angle-90)*0.0174);
  byte x2 = round(x0 + joueur.taille*cos(angle_rad));
  byte y2 = round(y0 + joueur.taille*sin(angle_rad));
  byte x3 = round(x1 + joueur.taille*cos(angle_rad));
  byte y3 = round(y1 + joueur.taille*sin(angle_rad));
  byte x4 = round(x0 - joueur.taille*cos(angle_rad));
  byte y4 = round(y0 - joueur.taille*sin(angle_rad));
  byte x5 = round(x1 - joueur.taille*cos(angle_rad));
  byte y5 = round(y1 - joueur.taille*sin(angle_rad));
  gb.display.drawLine(x2, y2, x3, y3);
  gb.display.drawLine(x3, y3, x5, y5);
  gb.display.drawLine(x4, y4, x5, y5);
  gb.display.drawLine(x4, y4, x2, y2);
  if(joueur.hit > 0){
    if(TAILLE_BATON-joueur.taille < 6){
      gb.display.drawCircle(joueur.circle_x, joueur.circle_y, 2*(TAILLE_BATON-joueur.taille));
    }
    if(joueur.taille<6){
      drawExplosion(5-joueur.taille,JOUEUR_ECRAN_X,JOUEUR_ECRAN_Y);
    }
  }
}




void updateJoueur(){

  joueur.angle = (joueur.angle + 3*(1-2*joueur.reverse) + 180) % 180;

  if(joueur.hit > 0){ 
    joueur.angle = (joueur.angle + 18*(1-2*joueur.reverse) + 180) % 180;
    if(joueur.hit < 200) joueur.hit++;
    if(joueur.taille >= 0){ 
      joueur.taille--;
    }
    if(gb.buttons.pressed(BTN_A) || gb.buttons.pressed(BTN_B)){
      initNiveau(current_num_level);
      gb.sound.playOK();
    }
    if(gb.buttons.pressed(BTN_C)){
      mode = MENU;
      menu_frames = 0;
      initMenu(current_num_level);
      gb.sound.playCancel();
    }

  }
  if(joueur.goal){
    joueur.angle = (joueur.angle + 9*(1-2*joueur.reverse) + 180) % 180;
    if(joueur.goal < 200) joueur.goal++;
    if(joueur.x > joueur.x_goal){
      joueur.x -= min(2,joueur.x-joueur.x_goal); 
    }
    if(joueur.x < joueur.x_goal){
      joueur.x += min(2,joueur.x_goal-joueur.x); 
    }
    if(joueur.y > joueur.y_goal){
      joueur.y -= min(2,joueur.y-joueur.y_goal); 
    }
    if(joueur.y < joueur.y_goal){
      joueur.y += min(2,joueur.y_goal-joueur.y); 
    }
  }
  if(joueur.hit == 0  && !joueur.goal){
    if(joueur.start) joueur.timer = min(2000,joueur.timer + 1);

    joueur.x_diff = 0;
    joueur.y_diff = 0;

    if(!gb.buttons.repeat(BTN_UP, 1) && joueur.derniere_direction == 3 || !gb.buttons.repeat(BTN_DOWN, 1) && joueur.derniere_direction == 2){
      if(gb.buttons.repeat(BTN_RIGHT, 1) && !gb.buttons.repeat(BTN_LEFT, 1)) joueur.derniere_direction = 0;
      if(!gb.buttons.repeat(BTN_RIGHT, 1) && gb.buttons.repeat(BTN_LEFT, 1)) joueur.derniere_direction = 1;
    }
    if(!gb.buttons.repeat(BTN_RIGHT, 1) && joueur.derniere_direction == 0 || !gb.buttons.repeat(BTN_LEFT, 1) && joueur.derniere_direction == 1){
      if(gb.buttons.repeat(BTN_UP, 1) && !gb.buttons.repeat(BTN_DOWN, 1)) joueur.derniere_direction = 3;
      if(!gb.buttons.repeat(BTN_UP, 1) && gb.buttons.repeat(BTN_DOWN, 1)) joueur.derniere_direction = 2;
    }

    if(gb.buttons.pressed(BTN_RIGHT)) joueur.derniere_direction = 0;
    if(gb.buttons.pressed(BTN_LEFT)) joueur.derniere_direction = 1;
    if(gb.buttons.pressed(BTN_DOWN)) joueur.derniere_direction = 2;
    if(gb.buttons.pressed(BTN_UP)) joueur.derniere_direction = 3;

    if(gb.buttons.repeat(BTN_RIGHT, 1) && joueur.derniere_direction == 0){
      joueur.x_diff = VITESSE_DROITE; 
    }
    if(gb.buttons.repeat(BTN_LEFT, 1) && joueur.derniere_direction == 1){
      joueur.x_diff = -VITESSE_DROITE; 
    }
    if(gb.buttons.repeat(BTN_DOWN, 1) && joueur.derniere_direction == 2){
      joueur.y_diff = VITESSE_DROITE; 
    }
    if(gb.buttons.repeat(BTN_UP, 1) && joueur.derniere_direction == 3){
      joueur.y_diff = -VITESSE_DROITE; 
    }

    /* MOUVEMENT EN DIAGONALE, abandonné
     if(gb.buttons.repeat(BTN_RIGHT, 1) && !gb.buttons.repeat(BTN_LEFT, 1)){//Mouvement Droite
     if(gb.buttons.repeat(BTN_UP, 1) ^ gb.buttons.repeat(BTN_DOWN, 1)){//Mouvement diagonal
     joueur.x_diff = VITESSE_DIAG; 
     if(gb.buttons.repeat(BTN_UP, 1)){
     joueur.y_diff = -VITESSE_DIAG; 
     }
     if(gb.buttons.repeat(BTN_DOWN, 1)){
     joueur.y_diff = VITESSE_DIAG; 
     }
     }
     else{
     joueur.x_diff = VITESSE_DROITE; 
     
     }
     }
     if(!gb.buttons.repeat(BTN_RIGHT, 1) && gb.buttons.repeat(BTN_LEFT, 1)){//Mouvement Gauche
     if(gb.buttons.repeat(BTN_UP, 1) ^ gb.buttons.repeat(BTN_DOWN, 1)){//Mouvement diagonal
     joueur.x_diff = -VITESSE_DIAG; 
     if(gb.buttons.repeat(BTN_UP, 1)){
     joueur.y_diff = -VITESSE_DIAG; 
     }
     if(gb.buttons.repeat(BTN_DOWN, 1)){
     joueur.y_diff = VITESSE_DIAG; 
     }
     }
     else{
     joueur.x_diff = -VITESSE_DROITE; 
     
     }
     }
     if(!(gb.buttons.repeat(BTN_RIGHT, 1) ^ gb.buttons.repeat(BTN_LEFT, 1))){
     if(gb.buttons.repeat(BTN_UP, 1) ^ gb.buttons.repeat(BTN_DOWN, 1)){//Mouvement diagonal
     if(gb.buttons.repeat(BTN_UP, 1)){
     joueur.y_diff = -VITESSE_DROITE; 
     }
     if(gb.buttons.repeat(BTN_DOWN, 1)){
     joueur.y_diff = VITESSE_DROITE; 
     }
     }
     }
     */

    if(!joueur.start &&  (joueur.x_diff != 0 || joueur.y_diff != 0)){ joueur.start = 1; joueur.angle_laststart = joueur.angle;}

    joueur.x_full += joueur.x_diff;
    while(verifcollisionRessorts() && joueur.x_diff != 0) joueur.x_full -= joueur.x_diff/4;
    joueur.y_full += joueur.y_diff;
    while(verifcollisionRessorts()&& joueur.y_diff != 0) joueur.y_full -= joueur.y_diff/4;

    joueur.x=joueur.x_full/5;
    joueur.y=joueur.y_full/5;

    //Arrivée
    if(joueur.goal == 0 && joueur.hit == 0 && abs(joueur.x-joueur.x_goal)<9 && abs(joueur.y-joueur.y_goal)<9){
      joueur.goal = 1; 
    }
  }

}

boolean verifcollisionRessorts(){//éviter que le mouvement ne permette au joueur de toucher un ressort par le centre
  boolean ret = false;
  for(byte i = 0; i <  joueur.max_ressorts; i++){
    if(pgm_read_byte_near(current_settings  + 5+i*3)-(joueur.x_full/5) <= 2 && pgm_read_byte_near(current_settings  + 5+i*3)-(joueur.x_full/5) > -10 &&
      pgm_read_byte_near(current_settings  + 6+i*3)-(joueur.y_full/5) <= 2 && pgm_read_byte_near(current_settings  + 6+i*3)-(joueur.y_full/5) > -10){
      ret = true;
    }
  }
  return ret;
}














