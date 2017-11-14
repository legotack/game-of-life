/**
	File: lifeController.h
	Author: Jack Seibert
	
	LifeController class wraps around a LifeGrid, and controls
	evolution. Also is responsible for setting up the grid, and
	has a method to output the grid via text.
*/

#pragma once

#include <iostream> // for ostream
#include "lifeGrid.h"

class LifeController {
public:
	/* Constructors:
	   -------------
	   Sets up the LifeController for a given grid. If
	   an input stream is specified, input will be parsed
	   as a list of coordinates to set initial alive cells.
	   Format is as follows:
	   		(1, 2)
	   		(2, 3)
	   		(-2000000000, 5)
	*/
	LifeController(LifeGrid * const gridIn);
	LifeController(LifeGrid * const gridIn, std::istream& input);

	/* Function: nextGeneration
	   ------------------------
	   Called to progress the grid to the next generation,
	   given the standard rules of Conway's Game of Life.
	*/
	void nextGeneration() const;

	/* Function: outputWorld
	   ---------------------
	   Outputs the current world to the given stream. Dead
	   cells are represented as '-', alive cells as 'X', and
	   the center as 'O'.
	*/
	void outputWorld(std::ostream& stream) const;
private:
	LifeGrid * const grid; // pointer to the LifeGrid object, which contains all the information on cells.

	/* Function: aliveNextGen
	   ----------------------
	   Returns true if the given cell would be alive next generation, or false
	   otherwise.
	*/
	bool aliveNextGen(const coordinate cell) const;
};