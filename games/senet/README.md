# Senet

Senet is an adaptation of the ancient board game for the Gamebuino.
It features a complete game play with a simple AI. As this game requires 
fortune besides a strategy to win, I think the AI is not that bad.

Implemented features:

 * single player game against simple AI
 * hot seat multi player
 * multi player via I2C (still BETA: somewhat buggy)

Senet is a board game played in ancient Egypt. Therefore it is not easy to
reconstruct the rules. There are many different versions out there and I
used the results on a survey [1] to set the rules of this implementation. 
Thankfully, these rules were easy to implement.


[1]: http://www.boardgamegeek.com/thread/478412/rules-rules-rules-how-do-you-play


Rules
=====

### 1. Game Setup

When starting the game you can choose how many every player gets (min 3,  max 7).
Which piece you will get is shown in the "Game Setup" next to "Your pieces:":

    +-+ +-+ +-+                     +-+     +-+
    | |_| |_| |                     | |_____| |
    |__     __|                     |___   ___|
       |   |            OR             /   \
       |   |                          /     \
       |   |                         /       \
       |___|                        |_________|
Which one you get is randomly chosen.

The pieces are placed on the board alternating, starting from square 1.

The board looks like this:

    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
    |     |     |     |     |     |     |     |     |     |     |
    |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  9  |  10 |
    |     |     |     |     |     |     |     |     |     |     |
    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
    |     |     |     |     |     |  O  |     |     |     |     |
    |  20 |  19 |  18 |  17 |  16 |  +  |  14 |  13 |  12 |  11 |
    |     |     |     |     |     |  |  |     |     |     |     |
    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
    |     |     |     |     |     | +++ | ~~~ |     |     |     |
    |  21 |  22 |  23 |  24 |  25 | OOO | ~~~ | III |  II |  30 |
    |     |     |     |     |     |     | ~~~ |     |     |     |
    +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+

Special names of some squares:
15: House of Rebirth
27: House of Water / Water Trap


### 2. Game Start
The first move in every game MUST move the piece on the highest square. E.g. if
you are playing with 3 pieces per player, the piece on square 6 must be moved 
by the first move.

### 3. Game Play
Every turn starts by throwing the paddles (done automatically by the game, see
the blinking rectangle above the board). There a set of four paddles which have 
two sides: a black and a white one. Your step count (short: steps) is determine
by the number of white sides you throw. There is a special rule: If you get 4
black sides, you get 6 steps.

If you get 1, 4 or 6 moves, you also get the next turn which means you have to 
throw again. You could say that your opponent passes his next turn.

The relation between white sides and steps is shown in this table:


 white sides | step count | throw again |probability
 ----------: | ---------: | ----------: | -----------:
(4x black) 0 |          6 |         Yes |     6.25% 
           1 |          1 |         Yes |     25.00% 
           2 |          2 |          No |     37.50% 
           3 |          3 |          No |     25.00% 
           4 |          4 |         Yes |      6.25% 



Every turn you must choose one of your pieces you want to move by your steps.
You cannot split your steps between multiple pieces. Usually you are moving your
pieces forward but if it is not possible for any of your pieces to move 
forward, you need to move backwards (according to the same rules). If even 
that is impossible your turn is skipped.

If you land on one of your opponent's pieces you exchange places this is 
called an attack. You cannot attack your own pieces. You need to search for 
another move then.

There are some restrictions for your moves:

 * Safe squares: Squares 15, 26, 28 and 29 are safe. You cannot attack a piece 
    standing on one of these squares.
 * Defence: Two or more opponent pieces in a row (consecutive) cannot be 
    attacked. They defend each other.
 * Blockade: Three or more pieces in a row cannot be passed they block your way.
    But blockades cannot turn around corners (sq. 10-11 / 20-21)
 * Trap: There is a trap of water on square 26. If you fall into it your piece 
    goes back to square 15 (House of Rebirth) or the first free square before 
    (sq. 14 to 1) as you are not allowed to attack an opponent's piece while 
    you are dead. ;)
 * Exit: You may not move past square 30. A piece on square 30 can be removed at
    the START of your turn if all your pieces are out of the first row.
    

### 4. Goal
The goal is to get all your pieces to field 30 and remove them from the board.
The player who is the first to remove all his pieces wins the game.


Compilation
===========

To compile the game you need the Arduino IDE and the Gamebuino library.

Place (at least) all *.ino, *.h, *.cpp files into a folder named "Senet". You
can also rename the folder containing "Senet.ino" and the other source files.
Renaming is the easiest way to go there. ;)

Then you can open "Senet.ino" using the Arduino IDE. The game should compile 
and upload like any Arduino sketch.


