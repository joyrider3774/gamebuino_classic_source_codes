--------------------------------------------------
A to K (A2K) - a 2048 clone puzzle for Gamebuino
v1.0b, C. Mari, Nov 2014, www.carloslabs.com
--------------------------------------------------

A to K (in this document referred as A2K) is released
under a Creative Commons licence

http://creativecommons.org/licenses/by/4.0/

You are allowed to use, modify and redistribute this
software, but not sell it.

Attribution — You must give appropriate credit, 
provide a link to the license, and indicate if changes
were made. You may do so in any reasonable manner, 
but not in any way that suggests the licensor endorses
you or your use.

--------------------------------------------------

A2K
===

1) Mechanics of the game
------------------------

The objective of the game is to move tiles around the board
using the four directional buttons. When a tile hits against
a tile with a similar value, one of the tiles is promoted to
the next letter up, and the other tile disappears.

A hits A: one tile becomes B and one tile vanishes
B + B: C
C + C: D... and so on

This version of the puzzle calculates all the values in each
row simultaneously (*):

For example, the following combinations are possible

B + B = C
A + A + B = C
A + A + A + A = C

The game is won once you reach all the way up to K

The game is lost is all the positions in the board are filled (*)

If a tile hits another with a different value, both stay in their
positions.

If a tile can be moved in the chosen direction, the tile will
slide until it hits another tile, or the edge of the board.

A random tile is placed on the board every time a directional button
is pressed -even if there is no movement on the board. (*)

(*) Other versions of 2048 may not include these rules - particularly
the "board is full" rule. 

Bottom line: don't let the board get full. Ever. A board full (even
with the same tile) will end the game

2) Controls
-----------

Directional buttons: slide tiles in a chosen direction
B-button: reset game
C-button: exit game and flash LOADER.HEX

3) Version
----------

This is a BETA version of A2K. No further features are planned for
A2K - it was created as a simple project to grasp the foundations
of Gamebuino programming over a few days.

You are free to add or modify the code as long as you abide by the
terms of the licence.

--------------------------------------------------
A to K (A2K) - a 2048 clone puzzle for Gamebuino
v1.0b, C. Mari, Nov 2014, www.carloslabs.com
--------------------------------------------------