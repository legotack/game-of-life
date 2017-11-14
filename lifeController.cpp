/**
	File: lifeController.cpp
	Author: Jack Seibert

	Implementation of LifeController class.
*/

#include "lifeController.h"
#include <regex> // for matching input
#include <string> // for stol

using namespace std;

/* regex pattern to match input in the form "(x, y)" */
const regex INPUT_PATTERN("\\((-?[0-9]+),\\s*(-?[0-9]+)\\)");

LifeController::LifeController(LifeGrid * const gridIn)
	: grid(gridIn) { }

LifeController::LifeController(LifeGrid * const gridIn, istream& input)
	: grid(gridIn) {
	string line;
	getline(input, line);
	while (line != "") {
		smatch matches;
		if (regex_match(line, matches, INPUT_PATTERN)) {
			grid->setAlive({ stol(matches[1]), stol(matches[2]) }, true);
		} else {
			throw "Invalid input: " + line;
		}
		if (input.eof()) break;
		getline(input, line);
	}
}

void LifeController::nextGeneration() const {
	// Begins by making a set of cells to explore,
	// because the only cells with a possibility of change are
	// those that are alive, and those that are neighbors of an
	// alive cell.
	cellSet cellsToExplore;
	cellSet aliveCells = grid->getAliveCells();
	for (auto it = aliveCells.begin(); it != aliveCells.end(); ++it) {
		for (long i = it->second - 1; i <= it->second + 1; ++i) {
			for (long j = it->first - 1; j <= it->first + 1; ++j) {
				cellsToExplore.insert({ j, i });
			}
		}
	}

	// Applies the rules of Conway's Game of Life to the world, and
	// overwrites the current grid with the new one.
	LifeGrid newGrid(grid->radius, grid->getNumBuckets());
	for (auto it = cellsToExplore.begin(); it != cellsToExplore.end(); ++it) {
		if (aliveNextGen(*it))
			newGrid.setAlive(*it, true);
	}
	*grid = newGrid;
}

bool LifeController::aliveNextGen(const coordinate cell) const {
	int aliveNeighbors = grid->countAliveNeighbors(cell);
	return aliveNeighbors == 3 || (aliveNeighbors == 2 && grid->isAlive(cell));
}

void LifeController::outputWorld(ostream& stream) const {
	stream << *grid << endl;
}