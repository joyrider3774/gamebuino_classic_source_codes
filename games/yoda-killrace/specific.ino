//----------------------------------------------------------------------------
void checkbuttons() {
  byte changed=0;
  if (gb.buttons.pressed(BTN_LEFT) and deadcounter==-1) {
    if (carxr != -2) {
      playsoundfx(0,0);
      changed=1;
    }
    carxr=-2;
    caryr=0;
    carshape=0;
  }      
  if (gb.buttons.pressed(BTN_RIGHT) and deadcounter==-1) {
    if (carxr != 2) {
      playsoundfx(0,0);
      changed=1;
    }
    carxr=2;
    caryr=0;
    carshape=2;
  }
  if (gb.buttons.pressed(BTN_UP) and deadcounter==-1) {
    if (caryr != -2) {
      playsoundfx(0,0);
      changed=1;
    }
    carxr=0;
    caryr=-2;
    carshape=1;
  }      
  if (gb.buttons.pressed(BTN_DOWN) and deadcounter==-1) {
    if (caryr != 2) {
      playsoundfx(0,0);
      changed=1;
    }
    carxr=0;
    caryr=2;
    carshape=3;
  }
  if (gb.buttons.pressed(BTN_A) and deadcounter==-1) {
    if (carxr !=0 or caryr != 0) {
      playsoundfx(0,0);
      score=--score;
      if (score<0) { score=0; }
    }
    carxr=0;
    caryr=0;
  }
  if (gb.buttons.pressed(BTN_C)) {
    gb.titleScreen(F("    Yoda's"),gamelogo);
    gamestatus="title";
  }
  if (changed==0 and deadcounter==-1 and yeahtimer==0 and (carxr != 0 or caryr != 0)) { playsoundfx(1,0); }
}
//----------------------------------------------------------------------------
void movecar() {
  gb.display.drawBitmap(0,0,background);
  dustcounter=++dustcounter % 4;
  if (dustcounter==0) {
    for (i=2 ; i<255 ; i--) {
      dustx[i+1]=dustx[i];
      dusty[i+1]=dusty[i];
    }
    dustx[0]=carx;
    dusty[0]=cary;
  }
  if (deadcounter==-1 and yeahtimer==0) {
    carx=carx+carxr;
    cary=cary+caryr;
    if (carx>200) {
      carx=0;
      carxr=0;
    }
    if (carx>76) {
      carx=76;
      carxr=0;
    }
    if (cary>200) {
      cary=0;
      caryr=0;
    }
    if (cary>40) {
      cary=40;
      caryr=0;
    }
  }
  if (deadcounter==-1) {
    gb.display.drawBitmap(carx,cary,carsprite[carshape]);
  } else {
    gb.display.drawBitmap(carx,cary,carsprite[carshape+8]);
    handledeath();
  }
  for (i=0; i<4; i++) {
    if (dustx[i] != carx or dusty[i] != cary) {
      gb.display.drawBitmap(dustx[i],dusty[i],carsprite[i+4]);
    }
  }
}
//----------------------------------------------------------------------------
void movemen() {
  //change men shape
  manframecounter=++manframecounter % manframemax;
  if (manframecounter==0) {
    manframe=++manframe % 2;
    for (i = 0; i<=mentokill; i++) {
      // change direction?        
      if (manxr[i]==-1 and manx[i]-4<carx+7 and manx[i]>carx+3 and manshape[i]==2) {
        manxr[i]=1;
        manshape[i]=0;
      }
      if (manxr[i]==1 and manx[i]+8>carx and manx[i]<carx and manshape[i]==0) {
        manxr[i]=-1;
        manshape[i]=2;
      }
      // move men
      if (manshape[i] != 10 and manshape[i] != 4) {
        manx[i]=manx[i]+manxr[i];
        many[i]=many[i]+random(3)-1;
        if (manx[i]>100) { manx[i]=80; }
        if (manx[i]>80) { manx[i]=0; }
        if (many[i]>100) { many[i]=0; }
        if (many[i]>41) { many[i]=41; }
      }
    }
  }
  for (i = 0; i<=mentokill; i++) {
    if (manshape[i] != 10) {
      if (manshape[i] == 4) {
        gb.display.drawBitmap(manx[i],many[i],mansprite[4]);
      } else {
        gb.display.drawBitmap(manx[i],many[i],mansprite[manshape[i]+manframe]);
      }
    }
  }
}
//----------------------------------------------------------------------------
void manappear() {
  manappearcounter=--manappearcounter;
  if (manappearcounter==0 and mancounter<mentokill) {
    manappearcounter=80-gamelevel*10;
    if (manappearcounter<10) { manappearcounter=10; }
    manx[mancounter]=random(2)*80;
    many[mancounter]=random(42);
    manxr[mancounter]=1;
    manshape[mancounter]=0;
    if (manx[mancounter]==80) {
      manxr[mancounter]=-1;
      manshape[mancounter]=2;
    }
    mancounter=mancounter+1;  
  }  
}
//----------------------------------------------------------------------------
void checkcollission() {
  if (deadcounter == -1) {
    for (i=0; i<=mentokill; i++) {
      if (manshape[i]==4) {
        // check collission with tombstone
        byte colflag=0;
        if (carxr<0 and manx[i]+5>=carx and manx[i]+3<carx and many[i]+6>=cary and many[i]<=cary+7) { colflag=1; }
        if (carxr>0 and manx[i]-1<=carx+7 and manx[i]+1>carx+7 and many[i]+6>=cary and many[i]<=cary+7) { colflag=1; }
        if (caryr<0 and many[i]+7>=cary and many[i]+5<cary and manx[i]+4>=carx and manx[i]<=carx+7) { colflag=1; }
        if (caryr>0 and many[i]-1<=cary+7 and many[i]+1>cary+7 and manx[i]+4>=carx and manx[i]<=carx+7) { colflag=1; }
        if (colflag == 1) {
          playsoundfx(3,1);
          deadcounter=50;
          manshape[i]=10;        
        }
      }
      if (manshape[i]==0 or manshape[i]==2) {
        // check collission with man 
        if (carx<=manx[i] and carx+7>=manx[i]+3 and cary<=many[i] and cary+7>=many[i]+6) {
          manshape[i]=4;
          score=score+25;  
          menkilled=menkilled+1;
          if (menkilled==mentokill) {
            playsoundfx(4,1);
          } else {
            playsoundfx(2,1);
          }        
        }
      }
    }
  }
}
