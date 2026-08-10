void collisionsJoueur(boolean springs){

  if(springs){//On teste s'il y a collision avec des ressorts, si oui on repositionne le bâton pour qu'il n'y en ait plus.
    if(testcollisionsJoueur(0, 0, 1)){
      testcollisionsprocheJoueur();
    } 
  }
  else{//On teste s'il y a collision avec des murs. Si oui, on teste les positions voisines qui ne causent pas de collision.
    if(testcollisionsJoueur(0, 1, 0)) {joueur.hit = 1; gb.sound.playPattern(destroy,0);}
    if(joueur.hit > 0){//Il vient d'y avoir collision. On cherche une position voisine qui ne cause pas de collision, pour déterminer le sens de rotation.
      testcollisionsprocheJoueur();
    } 
  }
}

void testcollisionsprocheJoueur(){
  boolean ok = false;
  char offset_collisions = -1;
  while(!ok && offset_collisions > -90){
    if(offset_collisions > 0) offset_collisions *= -1;
    else offset_collisions *= -3;
    if(!testcollisionsJoueur(offset_collisions*(1-2*joueur.reverse), 0,0)) ok = 1;
  }
  joueur.angle = (joueur.angle + offset_collisions*(1-2*joueur.reverse) + 180)%180;
  if(offset_collisions < 0) joueur.reverse = 1-joueur.reverse;
}

boolean testcollisionsJoueur(char offset, boolean first_collision, boolean activer_ressorts){
  //offset = pour tester un angle différent de celui du joueur, utile dans l'animation de fin et les ressorts
  //first_collision = sert à faire apparaître un cercle si collision il y a
  byte test_x1, test_x2, test_y1, test_y2;
  float angle_rad = ((joueur.angle + offset + 180) % 180)*0.0174;
  boolean ret = false;
  for(char i = joueur.taille; i>=1; i -= 2){
    test_x1 = JOUEUR_ECRAN_X+i*cos(angle_rad);
    test_x2 = JOUEUR_ECRAN_X-i*cos(angle_rad);
    test_y1 = JOUEUR_ECRAN_Y+i*sin(angle_rad);
    test_y2 = JOUEUR_ECRAN_Y-i*sin(angle_rad);
    if((gb.display.getPixel(test_x1, test_y1 ) ||  gb.display.getPixel(test_x2,test_y2 ))){
      if(first_collision){
        if(gb.display.getPixel(test_x1, test_y1 )){
          joueur.circle_x = test_x1; 
          joueur.circle_y = test_y1;
        }
        else{
          joueur.circle_x = test_x2; 
          joueur.circle_y = test_y2;
        }
      }
      if(activer_ressorts){//Si c'est une collision avec ressorts, détecter le numéro du ressort enfoncé
        byte coll_x,coll_y;
        if(gb.display.getPixel(test_x1, test_y1 )){
          coll_x = test_x1+joueur.x-JOUEUR_ECRAN_X; 
          coll_y = test_y1+joueur.y-JOUEUR_ECRAN_Y;
        }
        else{
          coll_x = test_x2+joueur.x-JOUEUR_ECRAN_X; 
          coll_y = test_y2+joueur.y-JOUEUR_ECRAN_Y;
        }
        gb.display.cursorY = 0;
        gb.display.cursorX = 0;
        for(byte i = 0; i <  joueur.max_ressorts; i++){
          if(pgm_read_byte_near(current_settings  + 5+i*3)-coll_x > -8 && pgm_read_byte_near(current_settings  + 5+i*3)-coll_x <= 0 && 
            pgm_read_byte_near(current_settings  + 6+i*3)-coll_y > -8 && pgm_read_byte_near(current_settings  + 6+i*3)-coll_y <= 0){
             animation_ressorts[i] = 1;
            }
        }
      }
      ret = true;
      return ret;
    }
  }
  return ret;
}









