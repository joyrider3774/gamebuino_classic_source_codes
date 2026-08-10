


//{x_départ, y_départ, x_arrivée, y_arrivée, nombre_ressorts, ressort_1_x, ressort_1_y, ressort_direction, ...}
//0 : haut, 1 : gauche, 2 : bas, 3 : droite 

PROGMEM prog_uchar *niveaux[] = 	   
{   
  niveau_0,
  niveau_1,
  niveau_2,
  niveau_3,
  niveau_4,
  niveau_5,
  niveau_6,
  niveau_7,};

PROGMEM prog_uchar *niveaux_settings[] = 	   
{   
  niveau_0_settings,
  niveau_1_settings,
  niveau_2_settings,
  niveau_3_settings,
  niveau_4_settings,
  niveau_5_settings,
  niveau_6_settings,
  niveau_7_settings,};


void initNiveau(byte lvl){
  current_num_level = lvl;//Chargement du bon niveau
  switch (current_num_level){
  case 1:
    {
      current_level = niveaux[1];
      current_settings = niveaux_settings[1];
    }
    break;
  case 2:
    {
      current_level = niveaux[2];
      current_settings = niveaux_settings[2];
    }
    break;
  case 3:
    {
      current_level = niveaux[3];
      current_settings = niveaux_settings[3];
    }
    break;
  case 4:
    {
      current_level = niveaux[4];
      current_settings = niveaux_settings[4];
    }
    break;
  case 5:
    {
      current_level = niveaux[5];
      current_settings = niveaux_settings[5];
    }
    break;
  case 6:
    {
      current_level = niveaux[6];
      current_settings = niveaux_settings[6];
    }
    break;
  case 7:
    {
      current_level = niveaux[7];
      current_settings = niveaux_settings[7];
    }
    break;
  default:
    {
      current_level = niveaux[0];
      current_settings = niveaux_settings[0];
    }
  }

  //Initialisation des variables du joueur
  joueur.x=pgm_read_byte_near(current_settings  + 0);
  joueur.y=pgm_read_byte_near(current_settings  + 1);
  joueur.x_goal=pgm_read_byte_near(current_settings  + 2);
  joueur.y_goal=pgm_read_byte_near(current_settings  + 3);
  joueur.x_full=joueur.x*5;
  joueur.y_full=joueur.y*5;
  joueur.angle = (joueur.angle_laststart + 140)%180;
  joueur.hit = 0;
  joueur.reverse = 0;
  joueur.taille = TAILLE_BATON;
  joueur.timer = 0;
  joueur.goal = 0;
  joueur.highscore = highscores[current_num_level];
  joueur.max_ressorts=pgm_read_byte_near(current_settings  + 4);
  joueur.start = 0;
  joueur.derniere_direction = -1;
  joueur.pause = 0;
  joueur.pause_off = 0;

  //Initialisation des variables d'animation des 32 ressorts
  for(byte i = 0; i<32; i++){
    animation_ressorts[i] = 0;
  }
}

void afficheDecor(){
  byte ca, decor;
  boolean decoraff;
  char drawX, drawY; 

  char oX = (joueur.x-JOUEUR_ECRAN_X)/32;//Origine en cases
  char oY = (joueur.y-JOUEUR_ECRAN_Y)/4;

  boolean skiplastX = 0;//Doit-on afficher 3 ou 4 barres?
  if((oX+3)*32 > joueur.x+JOUEUR_ECRAN_X) skiplastX = 1;

  byte o = oY*8 + oX%8;//Origine dans le tableau

  //Affichage des blocs
  for (byte i=0; i<56; i++){
    ca =  o + i%4 + (i/4)*8;
    decor = pgm_read_byte_near(current_level  + ca); 
    drawY = 4*(ca/8)-(joueur.y-JOUEUR_ECRAN_Y);

    for(byte j = 0; j < 8; j++){
      decoraff = (decor >> j) & 1;
      drawX = 4*((ca%8)*8+7-j)-(joueur.x-JOUEUR_ECRAN_X);
      if(drawX > 88) {
        decoraff = 0;
      }
      if(decoraff){
        gb.display.fillRect(drawX,drawY, 4, 4);
      }
    }
    if(i%4 == 2 && skiplastX) i++;//Passer la dernière barre


  }


  //Affichage des bords du niveau
  if(joueur.x-JOUEUR_ECRAN_X < 0){
    gb.display.fillRect(0,0, JOUEUR_ECRAN_X-joueur.x, LCDHEIGHT);
  }
  if(joueur.y-JOUEUR_ECRAN_Y < 0){
    gb.display.fillRect(0,0, LCDWIDTH, JOUEUR_ECRAN_Y-joueur.y);
  }
  if(128-joueur.y-JOUEUR_ECRAN_Y < 0){
    gb.display.fillRect(0,LCDHEIGHT+(128-joueur.y-JOUEUR_ECRAN_Y), LCDWIDTH, joueur.y+JOUEUR_ECRAN_Y-128);
  }
  if(256-joueur.x-JOUEUR_ECRAN_X < 0){
    gb.display.fillRect(LCDWIDTH+(256-joueur.x-JOUEUR_ECRAN_X),0, joueur.x+JOUEUR_ECRAN_X-256, LCDHEIGHT);
  }


}
void afficheArrivee(){
  //Affichage de l'arrivée
  switch(gb.frameCount % 4){
  case 0:
    {
      gb.display.drawBitmap(JOUEUR_ECRAN_X-5-(joueur.x-joueur.x_goal),JOUEUR_ECRAN_Y-5-(joueur.y-joueur.y_goal),goal,NOROT,NOFLIP);
    }
    break;
  case 1:
    {
      gb.display.drawBitmap(JOUEUR_ECRAN_X-5-(joueur.x-joueur.x_goal),JOUEUR_ECRAN_Y-5-(joueur.y-joueur.y_goal),goal2,NOROT,NOFLIP);
    }
    break; 
  case 2:
    {
      gb.display.drawBitmap(JOUEUR_ECRAN_X-5-(joueur.x-joueur.x_goal),JOUEUR_ECRAN_Y-10-(joueur.y-joueur.y_goal),goal,ROTCCW,NOFLIP);
    }
    break; 
  default:
    {
      gb.display.drawBitmap(JOUEUR_ECRAN_X-5-(joueur.x-joueur.x_goal),JOUEUR_ECRAN_Y-10-(joueur.y-joueur.y_goal),goal2,ROTCCW,NOFLIP);
    }
    break;  


  }
}

void afficheRessorts(){

  for(byte i = 0; i <  joueur.max_ressorts; i++){
    byte sens =pgm_read_byte_near(current_settings  + 7+i*3);
    switch (animation_ressorts[i]){
    case 0:
      {
        gb.display.drawBitmap(JOUEUR_ECRAN_X-(joueur.x-pgm_read_byte_near(current_settings  + 5+i*3)),JOUEUR_ECRAN_Y-(joueur.y-pgm_read_byte_near(current_settings  + 6+i*3)),ressorts[0],sens,NOFLIP);
      }
      break;
    case 4:
      {
        gb.display.drawBitmap(JOUEUR_ECRAN_X-(joueur.x-pgm_read_byte_near(current_settings  + 5+i*3)),JOUEUR_ECRAN_Y-(joueur.y-pgm_read_byte_near(current_settings  + 6+i*3)),ressorts[1],sens,NOFLIP);
      }
      break; 
    case 2:
      {
        gb.display.drawBitmap(JOUEUR_ECRAN_X-(joueur.x-pgm_read_byte_near(current_settings  + 5+i*3)),JOUEUR_ECRAN_Y-(joueur.y-pgm_read_byte_near(current_settings  + 6+i*3)),ressorts[2],sens,NOFLIP);
      }
      break; 
    default:
      {
        gb.display.drawBitmap(JOUEUR_ECRAN_X-(joueur.x-pgm_read_byte_near(current_settings  + 5+i*3)),JOUEUR_ECRAN_Y-(joueur.y-pgm_read_byte_near(current_settings  + 6+i*3)),ressorts[3],sens,NOFLIP);
      }
      break;  
    }
  }
  for(byte i = 0; i<32; i++){
    if(animation_ressorts[i] > 0) animation_ressorts[i]++;
    if(animation_ressorts[i] > 4) animation_ressorts[i] = 0;
  }
}












