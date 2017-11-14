/**
	File: lifeGrid.h
	Author: Jack Seibert

	LifeGrid represents a world in Conway's Game
	of Life. Backed by a set of alive cells, to avoid
	storing information on the vast number of dead cells.
	Coordinates out of bounds will wrap around. This allows
	infinite patterns (such as gliders) to continue endlessly.
*/

#pragma once

#include <iostream>
#include <unordered_set>
#include <boost/functional/hash.hpp> // for hashing pair

// A type for handling coordinates within the grid.
using coordinate = std::pair<long, long>;
// A type for storing a set of coordinates, treated as a set of cells.
using cellSet = std::unordered_set<coordinate, boost::hash<coordinate>>;

// A struct to represent the boundaries of the world. Inclusive, so
// a cell could be at (minX, maxY) for example.
struct boundaries {
	long minX, maxX, minY, maxY;
};

class LifeGrid {
public:
	/* Constructors:
	   -------------
	   Sets up the grid, given a radius. Note that not much needs to be
	   done in the constructor, because the implementation does not store
	   information for each cell, only the total bounds of the world and
	   the set of alive cells.
	*/
	LifeGrid(const size_t radiusIn);
	// This constructor sets up the unordered_set with a certain number of
	// initial buckets. This is useful for making a new grid that will likely
	// have a similar number of alive cells as a previous one.
	LifeGrid(const size_t radiusIn, const size_t numBuckets);
	// Copy constructor
	LifeGrid(const LifeGrid &copy);

	boundaries bounds; // The boundaries of the world.
	size_t radius; // The radius of the world.

	/* Function: isAlive
	   -----------------
	   Returns true if the specified cell is alive, false otherwise.
	*/
	bool isAlive(const coordinate cell) const;
	/* Function: setAlive
	   ------------------
	   Sets the given cell to be either alive or dead, given by alive parameter.
	*/
	void setAlive(const coordinate cell, const bool alive);

	/* Function: countAliveNeighbors
	   -----------------------------
	   Returns the number of neighbors (lying in the eight cardinal directions)
	   to the given cell that are alive.
	*/
	int countAliveNeighbors(const coordinate cell) const;

	/* Function: getAliveCells
	   -----------------------
	   Returns a set of all alive cells.
	*/
	cellSet getAliveCells() const;
	/* Function: getNumBuckets
	   -----------------------
	   Returns the number of buckets used in the unordered_set
	   storing cells.
	*/
	size_t getNumBuckets() const;
private:
	cellSet aliveSet; // The set of alive cells.

	/* Function: wrapCoordinate
	   ------------------------
	   Given a coordinate, returns a coordinate within the world
	   boundaries, wrapping around the edge if need be.
	*/
	coordinate wrapCoordinate(const coordinate cell) const;

	// Marked as a friend function to allow acces to aliveSet
	friend std::ostream& operator<<(std::ostream& stream, const LifeGrid& gr);
};

/* Function: operator<<
   --------------------
   Overloads the << operator to define how the grid should be written
   to a stream. Dead cells are represented as '-', alive cells as 'X', and
   the center as 'O'.
*/
std::ostream& operator<<(std::ostream& stream, const LifeGrid& gr);