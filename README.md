# game-of-life
Implements Conway's Game of Life in C++. Requires SDL2 and Boost, which can be downloaded here:

SDL2: https://www.libsdl.org/download-2.0.php
Boost: http://www.boost.org/users/history/version_1_65_1.html

Feel free to edit the Makefile to point to where you have installed these libraries.
(I was having issues statically linking to create a standalone binary. Let me know if downloading Boost and SDL2 presents any issues.)

Usage:

./game-of-life [radius] [input_file]

Radius defines the number of cells, from the center, that will be created in the world. (Note that the world wraps around at edges, so even if an input greater than the bounds of the world is input, it will still be valid.)
Input file should be a newline-separated list of coordinates at which to place alive cells.

A few sample input files are included in the "samples" directory, as well as a small python script that will generate random patterns.

Controls:

Space to pause evolution.
WASD to pan the camera.
Up and Down arrow keys to zoom in and out.
Left and Right arrow keys to speed up and slow down the evolution.
Left click to turn a cell alive, right click to make it die.
Z to autozoom to a random alive cell. Useful for huge grids where you may not be able to see the alive cells from a wide camera view.