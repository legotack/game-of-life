/**
	Defines the LifeGrid type for handling
	Conway's Game of Life. Backed by

	Author: Jack Seibert
	Date: 11/9/17
*/

#pragma once

#include <iostream>
#include <unordered_set>
#include <boost/functional/hash.hpp>

// row and column may be reversed

// ADD THIS IN EVENTAULLY!
//using cells = unsigned char; // each bit corresponds to one cell
using coordinate = std::pair<long, long>;
using setType = std::unordered_set<coordinate, boost::hash<coordinate>>;

struct boundaries {
	long minX, maxX, minY, maxY;
};

class LifeGrid {
public:
	LifeGrid(std::istream& input, size_t radius);
	LifeGrid(LifeGrid &copy);

	bool isAlive(const long x, const long y) const;
	void setAlive(const long x, const long y, const bool alive);

	void tick();
private:
	setType aliveSet;
	boundaries bounds;

	int countAliveNeighbors(const long row, const long col) const;

	friend std::ostream& operator<<(std::ostream& stream, const LifeGrid& gr);
};

std::ostream& operator<<(std::ostream& stream, const LifeGrid& gr);