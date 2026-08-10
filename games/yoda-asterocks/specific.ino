//----------------------------------------------------------------------------
void handleplayership() {
  if (playershipvisible==1) {
    //draw playership
    if (deadtimer==-1) {
      //move playership
      playershipx=playershipx+playershipxspeed;
      playershipy=playershipy+playershipyspeed;
      //draw ship
      gb.display.drawBitmap(playershipx/8,playershipy/8,playership[playershiprotation]);  
      // check buttons
      if (gb.buttons.repeat(BTN_B,0)) { // thrust
        playsoundfx(8,0);
        playershipxspeed=playershipxspeed+xadd[playershiprotation]/8;
        playershipyspeed=playershipyspeed+yadd[playershiprotation]/8;
        if (playershipxspeed>8) { playershipxspeed=8; }
        if (playershipxspeed<-8) { playershipxspeed=-8; }
        if (playershipyspeed>8) { playershipyspeed=8; }
        if (playershipyspeed<-8) { playershipyspeed=-8; }
      }      
      if (gb.buttons.repeat(BTN_DOWN,0)) {  // hyperspace
        playershipx=random(592)+40;
        playershipy=random(304)+40;
        playershipxspeed=0;
        playershipyspeed=0;
      }
      if (gb.buttons.repeat(BTN_LEFT,0)) { // rotate left
        playershiprotation=--playershiprotation;
        if (playershiprotation<0) { playershiprotation=playershiprotation+16; }
      }      
      if (gb.buttons.repeat(BTN_RIGHT,0)) { // rotate right
        playershiprotation=++playershiprotation % 16;
      }
      if (gb.buttons.pressed(BTN_A) and playershots<4) { // release shoot
        playershotxspeed[playershots]=xadd[playershiprotation];
        playershotyspeed[playershots]=yadd[playershiprotation];
        playershotx[playershots]=playershipx+16+playershotxspeed[playershots];
        playershoty[playershots]=playershipy+16+playershotyspeed[playershots];
        playershotcounter[playershots]=0;
        playershots=playershots+1;
        playsoundfx(0,0);
      }
    } else { // death
      handledeath();
    } // end of deadtimer
    i=++i;    
  } // end of ship visible
  
  // ship off screen -> appear at opposite side
  if (playershipx<-56) { playershipx=playershipx+664; }
  if (playershipx>664) { playershipx=playershipx-664; }
  if (playershipy<-56) { playershipy=playershipy+376; }
  if (playershipy>376) { playershipy=playershipy-376; }

  // player ship appears?
  if (playershipappear==1) { playershipvisible=1; }

  if (gb.buttons.pressed(BTN_C)) {
    gb.titleScreen(F("    Yoda's"),gamelogo);
    gamestatus="title";
  }
}
//----------------------------------------------------------------------------
void handleplayershots() {
  // move playershots
  i=0;
  while (i<playershots) {
    playershotx[i]=playershotx[i]+playershotxspeed[i];
    playershoty[i]=playershoty[i]+playershotyspeed[i];
    if (playershotx[i]<0) { playershotx[i]=playershotx[i]+664; }
    if (playershotx[i]>664) { playershotx[i]=playershotx[i]-664; }
    if (playershoty[i]<0) { playershoty[i]=playershoty[i]+376; }
    if (playershoty[i]>376) { playershoty[i]=playershoty[i]-376; }
    playershotcounter[i]=++playershotcounter[i];

    // draw playershots
    gb.display.drawBitmap(playershotx[i]/8,playershoty[i]/8,bullet);

    // remove shot if neccesary
    if (playershotcounter[i]>30) {
      playershotx[i]=playershotx[playershots-1];
      playershoty[i]=playershoty[playershots-1];
      playershotxspeed[i]=playershotxspeed[playershots-1];
      playershotyspeed[i]=playershotyspeed[playershots-1];
      playershotcounter[i]=playershotcounter[playershots-1];
      playershots=--playershots;
    }
    i=++i; // next playershot        
  }
}
//----------------------------------------------------------------------------
void handlerocks() {
  // move asterocks
  playershipappear=1;
  i=0;
  while (i<asterocksonscreen) {
    x=asterockxspeed[i]-8;
    y=asterockyspeed[i]-8;
    asterockx[i]=asterockx[i]+x;
    asterocky[i]=asterocky[i]+y;
    if (asterockx[i]<-80) { asterockx[i]=664; }
    if (asterockx[i]>664) { asterockx[i]=-80; }
    if (asterocky[i]<-80) { asterocky[i]=376; }
    if (asterocky[i]>376) { asterocky[i]=-80; }

    // draw asterocks
    gb.display.drawBitmap(asterockx[i]/8,asterocky[i]/8,asterocks[asterocktype[i]]);        

    // set collission offset depending on rock size
    left = 8 + 16*(asterocktype[i]>3) + 8*(asterocktype[i]>7);
    right = 72 - 16*(asterocktype[i]>3) - 8*(asterocktype[i]>7);

    // check collission with player ship
    if ((playershipx+8 < asterockx[i]+right) and (playershipx+40 > asterockx[i]+left) and (playershipy+8 < asterocky[i]+right) and (playershipy+40 > asterocky[i]+left) and playershipvisible==1 and deadtimer==-1) {
      deadtimer=40;          
      destroyed=1;          
    }

    // check collission with ufo
    if ((asterockx[i]+left < ufox+32+16*(ufotype==1)) and (asterockx[i]+right > ufox) and (asterocky[i]+left < ufoy+16+16*(ufotype==1)) and (asterocky[i]+right > ufoy) and ufotype != 0) {
      score=score+200+800*(ufotype==2);         
      destroyed=1;
      ufotype=0;
    }

    // check collission with ufoshot
    if ((ufoshotx < asterockx[i]+right) and (ufoshotx+16 > asterockx[i]+left) and (ufoshoty < asterocky[i]+right) and (ufoshoty+16 > asterocky[i]+left) and ufoshotx>-1) {
      destroyed=1;
      ufoshotx=-1;
    }

    // check collission with playershots
    u=0;
    while (u<playershots) {
      if ((playershotx[u] < asterockx[i]+right) and (playershotx[u]+16 > asterockx[i]+left) and (playershoty[u] < asterocky[i]+right) and (playershoty[u]+16 > asterocky[i]+left) and playershotcounter[u]<50) {
        destroyed=1;
        playershotcounter[u]=50;                         
      }
      u=++u;
    }

    //split or remove rock
    if (destroyed==1) {
      soundspeed=soundspeed-2*(soundspeed>5);
      playsoundfx(1+(asterocktype[i]>3)+(asterocktype[i]>7),2);
      destroyed=0;
      score=score+20+30*(asterocktype[i]>3)+50*(asterocktype[i]>7);
      if (asterocktype[i]<8) { //big or medium rock -> split
        asterockx[asterocksonscreen]=asterockx[i]+16;
        asterocky[asterocksonscreen]=asterocky[i]+16;           
        asterockx[i]=asterockx[i]+16;
        asterocky[i]=asterocky[i]+16;
        asterocktype[asterocksonscreen]=4+random(4)+4*(asterocktype[i]>=4);
        asterocktype[i]=4+random(4)+4*(asterocktype[i]>=4);
        asterockxspeed[asterocksonscreen]=rockxadd[random(12)]/4+8;
        asterockyspeed[asterocksonscreen]=rockyadd[random(12)]/4+8;
        asterockxspeed[i]=rockxadd[random(12)]/2+8;
        asterockyspeed[i]=rockyadd[random(12)]/2+8;
        if (asterockxspeed[i]==asterockxspeed[asterocksonscreen] and asterockyspeed[i]==asterockyspeed[asterocksonscreen]) {
          asterockxspeed[i]=-asterockxspeed[i];
          asterockyspeed[i]=-asterockyspeed[i];
        }
        if (asterocktype[asterocksonscreen]>7 or random(2)==1) {
          asterockxspeed[asterocksonscreen]=rockxadd[random(12)]/2+8;
          asterockyspeed[asterocksonscreen]=rockyadd[random(12)]/2+8;
        }
        if (asterocktype[i]>7 or random(2)==1) {
          asterockxspeed[i]=rockxadd[random(12)]/2+8;
          asterockyspeed[i]=rockyadd[random(12)]/2+8;
        }
        asterocksonscreen=asterocksonscreen+(asterocksonscreen<32);            
      } else { // small rock -> remove
        asterocksonscreen=--asterocksonscreen;
        asterockx[i]=asterockx[asterocksonscreen];
        asterocky[i]=asterocky[asterocksonscreen];
        asterocktype[i]=asterocktype[asterocksonscreen];
        asterockxspeed[i]=asterockxspeed[asterocksonscreen];
        asterockyspeed[i]=asterockyspeed[asterocksonscreen];
        if (asterocksonscreen==0) { // all rocks removed?
          yeahtimer=60;
        }
      }
    }

    // check if playership can appear (no rock in inside square)
    if ((asterockx[i]+right > 184 and asterockx[i]+left < 488) and (asterocky[i]+right > 72 and asterocky[i]+left < 312)) {
      playershipappear=0;
    }
    i=++i; // next asterock
  } // end of rock handling
}
//----------------------------------------------------------------------------
void ufoappears() {
  if (asterocksonscreen<8 and score>500 and ufotype==0 and random(250)<2) {
    ufotype=1+(random(4+gamelevel)>2); // which ufo?
    ufox=-56;
    ufoxr=3;
    if (random(2)==0) {
      ufox=672;
      ufoxr=-3;
    }        
    ufoy=random(320)+32;
    ufoyr=(random(3)-1)*3;
  }
}
//----------------------------------------------------------------------------
void moveufo() {
  if (ufotype != 0) {
    ufox=ufox+ufoxr;
    ufoy=ufoy+ufoyr;
    if (ufox % 5 == 0) { playsoundfx(4,3); }
    if (ufoy<-40) { ufoy=384; }
    if (ufoy>384) { ufoy=-40; }
    if (random(50)<2) { // change direction
      ufoyr=(random(3)-1)*3;
    }
    if (ufox<-56 or ufox>672) { ufotype=0; }
    gb.display.drawBitmap(ufox/8,ufoy/8,ufo[ufotype-1]);
    // check collission playership & ufo
    if ((ufox < playershipx+40) and (ufox+32+16*(ufotype==1) > playershipx+8) and (ufoy < playershipy+40) and (ufoy+16+16*(ufotype==1) > playershipy+8) and ufotype != 0 and playershipvisible==1 and deadtimer==-1) {
      ufotype=0;
      deadtimer=40;
      playsoundfx(1,2);
    }        
  }
}
//----------------------------------------------------------------------------
void playershotufocollission() {
  u=0;
  while (u<playershots) {
    if ((playershotx[u] < ufox+32+16*(ufotype==1)) and (playershotx[u]+16 > ufox) and (playershoty[u] < ufoy+16+16*(ufotype==1)) and (playershoty[u]+16 > ufoy) and playershotcounter[u]<50 and ufotype != 0) {
      playsoundfx(3,0);
      score=score+200+800*(ufotype==2);         
      ufotype=0;
      playershotcounter[u]=50;                         
    }
    u=++u;
  }
}
//----------------------------------------------------------------------------
void ufoshotrelease() {
  if (ufotype != 0 and ufoshotx == -1 and ufox>40 and ufox<608) {
    playsoundfx(9,0);
    ufoshotx=ufox+24;
    ufoshoty=ufoy+16;
    ufoshotxr=(random(3)-1)*8;
    ufoshotyr=(random(3)-1)*8;
    if (ufotype == 2) {
      ufoshotxr=-8+16*(ufox<playershipx);
      ufoshotyr=-8+16*(ufoy<playershipy);
    }
    if (ufoshotxr==0 and ufoshotyr==0) { ufoshotxr=8; }    
  }
}  
//----------------------------------------------------------------------------
void moveufoshot() {
  if (ufoshotx != -1) {
    ufoshotx=ufoshotx+ufoshotxr;
    ufoshoty=ufoshoty+ufoshotyr; 
    gb.display.drawBitmap(ufoshotx/8,ufoshoty/8,bullet);
    if (ufoshotx<0 or ufoshotx>664 or ufoshoty<0 or ufoshoty>376) {
      ufoshotx=-1;
    }
    // check collission ufoshot & player
    if ((ufoshotx < playershipx+40) and (ufoshotx+16 > playershipx+8) and (ufoshoty < playershipy+40) and (ufoshoty+16 > playershipy+8) and ufoshotx>-1 and playershipvisible==1 and deadtimer==-1) {
      ufoshotx=-1;
      deadtimer=40;
      playsoundfx(1,2);
    }
  }
}  
//----------------------------------------------------------------------------
void checkbonuslife() {
  if (score>=bonusscore) {
    playsoundfx(5,3);
    lives=++lives;
    bonusscore=bonusscore+10000;
      }
}
//----------------------------------------------------------------------------
void backgroundsound() {
  soundcounter++;
  if (soundcounter>soundspeed) {
    soundcounter=0;
    soundvalue=++soundvalue % 2;
    playsoundfx(soundvalue+6,1);
  }
}
