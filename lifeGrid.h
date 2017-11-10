/**
	Defines the LifeGrid type for handling
	Conway's Game of Life. Backed by

	Author: Jack Seibert
	Date: 11/9/17
*/

#pragma once

#include <iostream>
#include <map>

// row and column may be reversed

// ADD THIS IN EVENTAULLY!
//using cells = unsigned char; // each bit corresponds to one cell
using gridType = std::map<std::pair<long, long>, bool>;

class LifeGrid {
public:
	LifeGrid(std::istream& input);
	LifeGrid(LifeGrid &copy);

	void setCell(const long row, const long col, const bool alive);
	bool getCell(const long row, const long col) const;

	void tick();
private:
	gridType grid;
	long minRow, maxRow, minCol, maxCol;

	int countAliveNeighbors(const long row, const long col) const;

	friend std::ostream& operator<<(std::ostream& stream, const LifeGrid& gr);
};

std::ostream& operator<<(std::ostream& stream, const LifeGrid& gr);