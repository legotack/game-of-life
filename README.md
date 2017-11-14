# game-of-life
Implements Conway's Game of Life in C++, using SDL for graphics. Requires SDL and Boost. Feel free to edit the Makefile to refer to your 

Usage:

./game-of-life [radius] [input_file]

Input file should be a newline-separated list of coordinates to place alive cells.

Controls:

Space to pause evolution.
WASD to pan the camera.
Up and Down arrow keys to zoom in and out.
Left and Right arrow keys to speed up and slow down the evolution.
Left click to turn a cell alive, right click to make it die.
Z to autozoom to a random alive cell. Useful for huge grids where you may not be able to see the alive cells from a wide camera view.