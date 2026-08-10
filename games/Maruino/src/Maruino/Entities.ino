#define numEnt  10
Entity entities[10];
boolean collide;



void removeEnt(int i){
  entities[i].bitmap = PlayerC;
  entities[i].x = 0;
  entities[i].vx = 0;
  entities[i].y = 0;
  entities[i].vy = 0;
}

void clearEnts(){
  for(int i = 0; i<numEnt; i++){
    entities[i].x = entities[i].y = entities[i].vx = entities[i].vy = 0;
    entities[i].bitmap = PlayerC;
  }
}

void addEnt(int x, int y,const byte *b){
  for(int i = 0; i<numEnt; i++){
    if(entities[i].bitmap == PlayerC){
      entities[i].bitmap = b;
      entities[i].x = x;
      entities[i].y = y;
      entities[i].vy = 0;
      if(b == Mushroom){
        entities[i].vx = 1;
      }
      else{
        entities[i].vx = -1;
      }
      
      break;
    }
  }
}

void updateEnt(){
  for(int i = 0; i< numEnt; i++){
    if(entities[i].bitmap != PlayerC){
      
      entities[i].x += entities[i].vx;
      if(Collide(entities[i].x, entities[i].y, entities[i].bitmap)){
        entities[i].x -= entities[i].vx;
        entities[i].vx *= -1;
      }
      
      entities[i].vy += .4;
      if(entities[i].vy >2){
        entities[i].vy = 2;
      }
      
      entities[i].y += entities[i].vy;
      if(Collide(entities[i].x, entities[i].y, entities[i].bitmap)){
        entities[i].y -= entities[i].vy;
        entities[i].vy /= 2;
      }
      
      if(entities[i].y > LCDHEIGHT || entities[i].x - cameraPos <= -8 || entities[i].x - cameraPos > LCDWIDTH + 8){
        removeEnt(i);
      }
      
      if(entities[i].bitmap == Goomba && gb.frameCount%10 >= 5){
        gb.display.drawBitmap(entities[i].x - cameraPos, entities[i].y, entities[i].bitmap, NOROT, FLIPH);
      }
      
      else{
        gb.display.drawBitmap(entities[i].x - cameraPos, entities[i].y, entities[i].bitmap, NOROT, NOFLIP);
      }
      
    }
  }
}

boolean Collide(float x, float y, const byte* bMap){
  collide = false;
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
        case 'S' :
        case 'I' :
        case 'E' :
          block = Empty;
          break;
        default:
          break;
      }
      
      if(!(block == NULL)){
        if(gb.collideBitmapBitmap(x, y, bMap, j*8, LCDHEIGHT - (8*(6-i)), block)){
          collide = true;
        }
      }
    }
  }
  return collide;
}
