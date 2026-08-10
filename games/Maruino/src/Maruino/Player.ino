boolean canJump = false;
boolean walking = false;
boolean bounce = false;
const byte *character;

void startPlayer(){
  player.x = 0;
  player.y = 0;
  player.vx = 0;
  player.vy = 0;
  player.h = 4;
  player.invincible = 40;
  player.flip = false;
  player.big = false;
  player.crouch = false;
}


void updatePlayer(){
  walking = false;
  if(time == 0){//Time out
    die();
  }
  if(player.invincible > 0){//Invincibility frames
    player.invincible--;
  }
  
  if(gb.buttons.pressed(BTN_C)){//Main menu
    Menu();
  }
  if(gb.buttons.pressed(BTN_UP)){//Toggle display
    disp = !disp;
  }
  
  if(gb.buttons.timeHeld(BTN_B)){//Smart crouching
    if(player.big && !player.crouch){
      player.y += 8;
    }
    player.crouch = true;
  }
  
  else{
    if(player.big && player.crouch){//Checks if player can stand without colliding.
      player.y -= 8;
      if(playerCollide()){
        player.y += 8;
      }
      else{
        player.crouch = false;
      }
    }
  }
  


  player.vx *= 0.75;//So player.vx slows
  if(gb.buttons.repeat(BTN_RIGHT, 1)){//Move right
    player.vx += .4;
    player.flip = false;
    walking = true;
  }
  
  if(gb.buttons.repeat(BTN_LEFT,1)){//Move left
    player.vx -= .4;
    player.flip = true;
    walking = true;
  }
  
  if(player.big){
    if(player.crouch){
      character = PlayerC;
      player.h = 8;
    }
    else if(walking){//Walking animation
      player.h = 16;
      if(gb.frameCount % 12 >= 0 && gb.frameCount % 12 < 2){character = PlayerT1;}
      else if(gb.frameCount % 12 >= 2 && gb.frameCount % 12 < 4) { character = PlayerT2; }
      else if(gb.frameCount % 12 >= 4 && gb.frameCount % 12 < 6) { character = PlayerT3; }
      else if(gb.frameCount % 12 >= 6 && gb.frameCount % 12 < 8) { character = PlayerT4; }
      else if(gb.frameCount % 12 >= 8 && gb.frameCount % 12 < 10) { character = PlayerT5; }
      else if(gb.frameCount % 12 >= 10 && gb.frameCount % 12 < 12) { character = PlayerT6; }
    }
    else{
      character = PlayerT;
      player.h = 16;
    }
  }
  
  else{
    if(gb.frameCount % 6 >= 0 && gb.frameCount % 6 < 3 && walking){
      character = PlayerS1;
      player.h = 8;
    }
    else{
      character = PlayerS;
      player.h = 8;
    }
  }
  
  checkCode(code);
  
  if(player.vx > 2){
    player.vx = 2;
  }
  
  player.x += player.vx;
  if(playerCollide()){//Collision on x-axis
    player.x -= player.vx;
    player.vx = 0;
  }
  
  if(player.vy > 1){//Falling
    canJump = false;
  }
  if(canJump && (gb.buttons.timeHeld(BTN_A) > 0 && gb.buttons.timeHeld(BTN_A) < 5)){//Jump
    player.vy -= 1.3;
    if (gb.buttons.timeHeld(BTN_A) < 2){
      gb.sound.playOK();
    }
  }
  player.vy += 0.4;
  if(player.vy >5){
    player.vy = 2;
  }
  
  
  
  if(gb.buttons.released(BTN_A)){//Prevents player from jumping multiple times midair
    canJump = false;
  }
  
  player.y += player.vy;
  if(playerCollide()){//collision on y-axis
    if(player.vy > 0){
      canJump = true;
      for(int i = 0; i < player.vy; i++){
        player.y -= 1; 
        if(!playerCollide()){
          // We found the last open spot before the block
          player.vy = 0; // Stop moving
          break; //Quit the loop
        }
      }
    }
    else{
      player.y -= player.vy;
      player.vy /= 2;
    }
  }
  
  if(player.y > LCDHEIGHT){//fall to death
    die();
  }
  
  if(player.x + 4 - cameraPos > (int)(LCDWIDTH/2)  && cameraPos < (64*8)-(LCDWIDTH)){//Player is mid screen but screen isn't leaving the end of the level.
    cameraPos = player.x - LCDWIDTH/2 + 4;
  }
  
  if(player.x + 4 - cameraPos < (int)(LCDWIDTH/2) && cameraPos > 0){//Player is mid screen but screen isn't showing "before" the level
    cameraPos = player.x - LCDWIDTH/2 + 4;
  }
}

void drawPlayer(){
  if(player.invincible % 2 != 1){
    if(!player.flip){
      gb.display.drawBitmap(player.x - cameraPos, player.y, character, NOROT, NOFLIP);
    }
    else{
      gb.display.drawBitmap(player.x - cameraPos, player.y, character, NOROT, FLIPH);
    }
  }
}

void getBig(){
  if(!player.big){
    player.big = true;
    player.crouch = true;
  }
}

void getSmall(){
  if(player.invincible <= 0){
    player.invincible = 40;
    if(player.big){
      if(!player.crouch){
        player.y += 8;
      }
      player.big = false;
    }
    else{
      die();
    }
  }
}

void die(){
  if(lives == 0){
    lose();
  }
  else{
    lives --;
    time = 130;
    gb.popup(F("You died. \1"), 20);
    clearEnts();
    startPlayer();
    cameraPos = 0;
    loadLevel(curLev);
  }
}

boolean playerCollide(){
  collide = false;
  for(int k = 0; k<numEnt; k++){
    if(gb.collideBitmapBitmap(entities[k].x, entities[k].y, entities[k].bitmap, player.x, player.y, character)){
      if(entities[k].bitmap == Mushroom){
        getBig();
        score += 100;
        removeEnt(k);
      }
      
      if(entities[k].bitmap == Goomba){
        if(player.vy > 0 && player.y + player.h - 4 < entities[k].y){//Ugly code checking for players feet being above goomba
          removeEnt(k);
          bounce = true;
        }
        else{
          getSmall();
          if(player.invincible <= 0){
            entities[k].vx *= -1;
          }
        }
      }
    }
    
    if(entities[k].bitmap != PlayerC && player.invincible <= 0){
      collide = gb.collideBitmapBitmap(player.x, player.y, character, entities[k].x, entities[k].y, entities[k].bitmap);
    }
  }
  
  for(int i = 0; i<6; i++){
    for(int j = 0; j<64; j++){
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
        case 'I' ://Invisible
        case 'E' :
          block = Empty;
          break;
        case 'C' :
          block = Coin;
          break;
        case 'F' :
          block = Flag;
          break;
        case 'S' :
          block = Spikes;
          break;
        default:
          break;
      }
      
      if(block != NULL){
        if(gb.collideBitmapBitmap(player.x, player.y, character, j*8, LCDHEIGHT - (8*(6-i)), block)){
          if(block == Flag){
            nextLevel();
          }
          
          if(player.vy < 0 && player.y > LCDHEIGHT - (8*(5-i)) - 1 && (player.x < (j+1)*8 && player.x + 8 > j*8)){//Checks if under block 
            if(block == Brick && player.big){
              curLevel[i*64+j] = NULL;
            }
            if(block == MysteryBlockC){
              curLevel[(i-1)*64+j] = 'C';
              curLevel[i*64+j] = 'E';
            }
            if(block == MysteryBlockM){
              curLevel[i*64+j] = 'E';
              addEnt(j*8, LCDHEIGHT - (8 * (7-i)), Mushroom);
            }
          }
          
          if(block == Spikes){
            getSmall();
            collide = true;
          }
          
          if(block == Coin){
            score += 100;
            curLevel[i*64+j] = NULL;
            gb.sound.playOK();
            collide = false;
          }
          
          else{
            collide = true;
          }
          
        }
      }
    }
  }
  if(bounce){
    player.vy = -4;
    bounce = false;
  }
  return collide;
}



