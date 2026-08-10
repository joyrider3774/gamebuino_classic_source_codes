#include <SPI.h>
#include <Gamebuino.h>
#include <EEPROM.h>
// penalty for hitting walls
#define PENALTY 500
#define MAZE_WIDTH 16
#define MAZE_HEIGHT 18

Gamebuino gb;
extern const byte font3x5[];

const byte logo[] PROGMEM = {
64, 28,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
B00111111, B00001111, B11000001, B00011111, B11101100, B00110001, B11111111, B11111100, 
B00110001, B10001100, B00000011, B10000011, B00001100, B00110001, B01010101, B01010100, 
B00110000, B11001100, B00000010, B10000011, B00001100, B00110001, B00010101, B01010100, 
B00110000, B11001100, B00000110, B11000011, B00001100, B00110001, B01010000, B01010100, 
B00110000, B11001111, B10000110, B11000011, B00001111, B11110001, B01010101, B01010100, 
B00110000, B11001100, B00000110, B11000011, B00001100, B00110001, B00010101, B01010100, 
B00110000, B11001100, B00001111, B11100011, B00001100, B00110001, B01000101, B01010100, 
B00110000, B11001100, B00001100, B01100011, B00001100, B00110001, B01010101, B01000100, 
B00110001, B10001100, B00001100, B01100011, B00001100, B00110001, B01010101, B00010100, 
B00111111, B00001111, B11011000, B00110011, B00001100, B00110001, B11111111, B11111100, 
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
B00000000, B00000000, B11100000, B01110000, B01000000, B11111100, B11111100, B00000000, 
B00000000, B00000000, B11100000, B01110000, B11100000, B00001100, B11000000, B00000000, 
B00000000, B00000000, B11110000, B11110000, B10100000, B00011000, B11000000, B00000000, 
B00000000, B00000000, B11010000, B10110001, B10110000, B00011000, B11000000, B00000000, 
B00000000, B00000000, B11010000, B10110001, B10110000, B00110000, B11111000, B00000000, 
B00000000, B00000000, B11011001, B00110001, B10110000, B00110000, B11000000, B00000000, 
B00000000, B00000000, B11001001, B00110011, B11111000, B01100000, B11000000, B00000000, 
B00000000, B00000000, B11001111, B00110011, B00011000, B11100000, B11000000, B00000000, 
B00000000, B00000000, B11000110, B00110011, B00011000, B11000000, B11000000, B00000000, 
B00000000, B00000000, B11000110, B00110110, B00001101, B11111100, B11111100, B00000000, 
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000
};

typedef struct{
    int8_t x;
    int8_t y;
}Player;

int16_t score, bestScore;
boolean finished = false;

Player player;

int16_t mazeMap[MAZE_HEIGHT];

void drawMaze(){
    gb.display.drawRect(0,0,76,40);
    gb.display.drawRect(1,1,74,38);
    for (int8_t y = 0; y < MAZE_HEIGHT; y++){
        for (int8_t x = 0; x < MAZE_WIDTH; x++){
            if ((mazeMap[y] >> (MAZE_WIDTH - (x+1)) ) & 1){
                gb.display.drawRect((x+1)*4, (y+1)*2, 2, 2);
            }
        }
    }
}

void drawScores(){
    gb.display.cursorX = 3;
    gb.display.cursorY = LCDHEIGHT - 6;
    gb.display.print("SCORE:");
    gb.display.print(score);
    gb.display.print(" BEST:");
    gb.display.print(bestScore);
}

void drawPlayer(boolean dontFlash){
    int8_t x, y;
    x = player.x * 2 + 2;
    y = player.y * 2 + 2;
    if (dontFlash || (gb.frameCount / 2) % 2){
        gb.display.drawRect(x, y, 2, 2);
    }
}

boolean testClear(int8_t x,int8_t y){
    return !((mazeMap[y] >> (MAZE_WIDTH - (x+1)) ) & 1);
}

void randomiseMaze(){
    //clear the map (make all the walls solid with no gaps)
    for (int8_t i = 0; i < MAZE_HEIGHT; i++){
        mazeMap[i] = 0b1111111111111111;
    }
    
    // work across adding gaps as we go
    for (int8_t i = MAZE_WIDTH-1; i >= 0; --i){
        int8_t noOfGaps;
        
        //how many gaps in the column?
        switch (random(10)){
            case 9:
                noOfGaps = 3;
                break;
            case 8:
            case 7:
                noOfGaps = 2;
                break;
            default:
                noOfGaps = 1;
        }
        // excuse my crazy method for this.
        /* the first line  is a little confusing even to me.
         * It makes a number like: 1111111111011111
         * with the 0 corresponding to the column
         * we are modifying, then  int he loop we bitwise AND
         * that with a row in the maze to create a gap in the wall
         */
        int16_t row = ~( ((int16_t)1) << (i));
        for (int8_t j = 0; j < noOfGaps; j++){
            mazeMap[random(MAZE_HEIGHT)] &= row;
        }
    }
}

int16_t readBest(){
    int16_t a,b;
    a = (int16_t) EEPROM.read(0);
    b = (int16_t) EEPROM.read(1);
    int8_t written = EEPROM.read(2);
    
    // does the eeprom contsin a score?
    if (written != 42){
        return 0;
    }
    
    int16_t best = b;
    best = best | (a << 8);
    return best;
}

void writeBest(int16_t best){
    int8_t a,b;
    b = (int8_t) best;
    a = (int8_t) (best >> 8);
    EEPROM.write(0, a);
    EEPROM.write(1, b);
    // flag for testing if score exists
    EEPROM.write(2, 42);
}

void reset(){
    randomiseMaze();
    player.x = 0;
    player.y = MAZE_HEIGHT / 2;
    score = 10000;
    bestScore = readBest();
    finished = false;
}

void setup() {
    gb.begin();
    gb.titleScreen(F(" msevilgenius's"), logo);
    gb.pickRandomSeed();
    gb.display.setFont(font3x5);
    finished = true;
    reset();
}

void loop() {
    if (gb.update()){
        static byte moving = 0;
        if (moving) { moving--; }
        if (!finished && player.x >= MAZE_WIDTH*2) {
            finished = true;
            if (score > bestScore){
                bestScore = score;
                writeBest(bestScore);
            }
        } //have we finished the maze?
        
        // c to return to title screen
        if (gb.buttons.pressed(BTN_C)){
            gb.titleScreen(F(" msevilgenius's"),logo);
        }
        
        // b to reset anytime
        if (gb.buttons.pressed(BTN_B)){
            reset();
        }
        
        // press a button to reset when finished
        if (finished && gb.buttons.pressed(BTN_A)){
            reset();
        }
        
        if (score > -10000 && !finished){
            // decrease score every frame
            score -= 5; 
        }
        
        // movement (only if we haven't got to the end)
        if (!finished){
            if (gb.buttons.pressed(BTN_UP)){
                moving = 6;
                if (player.y >0 && (!(player.x%2)||testClear((player.x-1)/2,player.y-1))){
                    player.y -= 1;
                }else{
                    score -= PENALTY;
                }
            }
            if (gb.buttons.pressed(BTN_DOWN)){
                moving = 6;
                if (player.y+1 < MAZE_HEIGHT && (!(player.x%2)||testClear((player.x-1)/2,player.y+1))){
                    player.y += 1;
                }else{
                    score -= PENALTY;
                }
            }
            if (gb.buttons.pressed(BTN_RIGHT)){
                moving = 6;
                if (player.x%2 || testClear(player.x/2,player.y)){
                    if (!finished)
                            player.x += 1;
                }else{
                    score -= PENALTY;
                }
            }
        }
        
        // drawing functions
        drawMaze();
        drawScores();
        drawPlayer(moving || finished);
    }
}
