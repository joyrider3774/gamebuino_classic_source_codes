

void drawExplosion(byte frame, byte x, byte y){
  switch(frame)
  {
  case 5: 
    { 
      gb.display.drawPixel(x, y);
      gb.display.drawPixel(x+6, y+6);
      gb.display.drawPixel(x-6, y-6);
      gb.display.drawPixel(x+6, y-6);
      gb.display.drawPixel(x-6, y+6);
    }
    break;

  case 0: 
    { 
      gb.display.drawPixel(x, y);
      gb.display.drawPixel(x, y+1);
      gb.display.drawPixel(x, y-1);
      gb.display.drawPixel(x+1, y);
      gb.display.drawPixel(x-1, y);
    }
  case 4 : 
    {

      gb.display.drawPixel(x+3, y+3);
      gb.display.drawPixel(x-3, y+3);
      gb.display.drawPixel(x+3, y-3);
      gb.display.drawPixel(x-3, y-3);

      gb.display.drawPixel(x+5, y+5);
      gb.display.drawPixel(x-5, y+5);
      gb.display.drawPixel(x+5, y-5);
      gb.display.drawPixel(x-5, y-5);


      gb.display.drawPixel(x-4, y);
      gb.display.drawPixel(x+4, y);

      gb.display.drawPixel(x, y+4);
      gb.display.drawPixel(x, y-4);
    }
    break;
  case 1: 
    { 
      gb.display.drawPixel(x, y+1);
      gb.display.drawPixel(x-1, y+1);
      gb.display.drawPixel(x+1, y+1);
      gb.display.drawPixel(x, y-1);
      gb.display.drawPixel(x-1, y-1);
      gb.display.drawPixel(x+1, y-1);
      gb.display.drawPixel(x-1, y);
      gb.display.drawPixel(x+1, y);

      gb.display.drawPixel(x-2, y);
      gb.display.drawPixel(x-3, y);
      gb.display.drawPixel(x+2, y);
      gb.display.drawPixel(x+3, y);

      gb.display.drawPixel(x, y+2);
      gb.display.drawPixel(x, y+3);
      gb.display.drawPixel(x, y-2);
      gb.display.drawPixel(x, y-3);

      gb.display.drawPixel(x+2, y+2);
      gb.display.drawPixel(x-2, y+2);
      gb.display.drawPixel(x+2, y-2);
      gb.display.drawPixel(x-2, y-2);
    }
  case 3: 
    {
      gb.display.drawPixel(x-5, y);
      gb.display.drawPixel(x+5, y);

      gb.display.drawPixel(x, y+5);
      gb.display.drawPixel(x, y-5);

      gb.display.drawPixel(x+4, y+4);
      gb.display.drawPixel(x-4, y+4);
      gb.display.drawPixel(x+4, y-4);
      gb.display.drawPixel(x-4, y-4);
    }
    break;
  case 2: 
    { 
      gb.display.drawPixel(x, y+1);
      gb.display.drawPixel(x-1, y+1);
      gb.display.drawPixel(x+1, y+1);
      gb.display.drawPixel(x, y-1);
      gb.display.drawPixel(x-1, y-1);
      gb.display.drawPixel(x+1, y-1);
      gb.display.drawPixel(x-1, y);
      gb.display.drawPixel(x+1, y);

      gb.display.drawPixel(x-2, y);
      gb.display.drawPixel(x-4, y);
      gb.display.drawPixel(x+2, y);
      gb.display.drawPixel(x+4, y);

      gb.display.drawPixel(x, y+2);
      gb.display.drawPixel(x, y+4);
      gb.display.drawPixel(x, y-2);
      gb.display.drawPixel(x, y-4);

      gb.display.drawPixel(x+3, y+3);
      gb.display.drawPixel(x-3, y+3);
      gb.display.drawPixel(x+3, y-3);
      gb.display.drawPixel(x-3, y-3);
    }
    break;
  }
} 


