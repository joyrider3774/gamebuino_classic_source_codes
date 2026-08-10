void updateCredits(){
  if(current_num_level == -2){//Déblocage
    gb.display.cursorX = 0;
    gb.display.cursorY = 0;
    gb.display.println(F("Do you want to"));
    gb.display.println(F("unlock ALL levels?"));
    gb.display.cursorY = 16;
    gb.display.println(F("This will also erase"));
    gb.display.println(F("your records."));
    gb.display.cursorY = 31;
    gb.display.cursorX = 14;
    gb.display.print(F("\25Let's do this!"));
    gb.display.cursorY = 38;
    gb.display.cursorX = 14;
    gb.display.print(F("\26No way!"));
    if(gb.buttons.pressed(BTN_A)){
      for(byte i = 0; i < NOMBRE_NIVEAUX; i++){
        highscores[i] = 0;
        complete[i] = 0;
      }
      unlocked_level = NOMBRE_NIVEAUX;
    }

  }
  else{//Crédits
    gb.display.cursorX = 0;
    gb.display.cursorY = 0;
    gb.display.println(F("Spin Spin Spinbuino !"));
    gb.display.cursorY = 8;
    gb.display.println(F("Created by:"));
    gb.display.cursorX = 8;
    gb.display.println(F("Charly Piva \"Zoglu\""));
    gb.display.cursorX = 8;
    gb.display.println(F("Margot Piva  \"Isil\""));
    gb.display.cursorX = 32;
    gb.display.println(F("www.zoglu.net"));
    gb.display.cursorY = 34;
    gb.display.cursorX = 0;
    gb.display.println(F("Thanks to:"));
    gb.display.cursorX = 16;
    gb.display.println(F("Rodot and Myndale"));

  }
  if(gb.buttons.pressed(BTN_A) ||gb.buttons.pressed(BTN_B) || gb.buttons.pressed(BTN_C)){
    mode = MENU;
    menu_frames = 0;
    initMenu(current_num_level);
    gb.sound.playCancel();
  }
}




