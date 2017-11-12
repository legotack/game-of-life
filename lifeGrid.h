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

using coordinate = std::pair<long, long>;
using setType = std::unordered_set<coordinate, boost::hash<coordinate>>;

struct boundaries {
	long minX, maxX, minY, maxY;
};

class LifeGrid {
public:
	LifeGrid(size_t radiusIn);
	LifeGrid(size_t radiusIn, size_t numBuckets);
	LifeGrid(LifeGrid &copy);

	boundaries bounds;
	size_t radius;

	bool isAlive(const long x, const long y) const;
	void setAlive(const coordinate cell, const bool alive);
	void setAlive(const long x, const long y, const bool alive); // LEAVE OPTION TO EXTEND WORLD FOR INPUT

	bool aliveNextGen(const long x, const long y) const;

	setType getAliveCells();
	size_t getNumBuckets();
private:
	setType aliveSet;

	coordinate wrapCoordinate(const long x, const long y) const;

	int countAliveNeighbors(const long x, const long y) const;

	friend std::ostream& operator<<(std::ostream& stream, const LifeGrid& gr);
};

std::ostream& operator<<(std::ostream& stream, const LifeGrid& gr);