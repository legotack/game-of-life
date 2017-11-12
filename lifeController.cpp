#include "lifeController.h"
#include <regex>
#include <string>
#include <sstream>

using namespace std;

long strToLong(string str);

/* regex pattern to match input in the form "(x, y)" */
const regex INPUT_PATTERN("\\((-?[0-9]+),\\s*(-?[0-9]+)\\)");

LifeController::LifeController(LifeGrid *gridIn)
	: grid(gridIn) { }

LifeController::LifeController(LifeGrid *gridIn, istream& input)
	: grid(gridIn) {
	string line;
	getline(input, line);
	while (!input.eof() && line != "") {
		smatch matches;
		if (regex_match(line, matches, INPUT_PATTERN)) {
			grid->setAlive(strToLong(matches[1]), strToLong(matches[2]), true);
		} else {
			// make this pretty
			throw "Invalid input: " + line;
		}
		getline(input, line);
	}
}

long strToLong(string str) {
	stringstream stream(str);
	long result;
	stream >> result;
	return result;
}

// see if you can do this smarter, to only look near alive cells
void LifeController::tick() {
	setType cellsToExplore;
	setType aliveCells = grid->getAliveCells();
	for (auto it = aliveCells.begin(); it != aliveCells.end(); ++it) {
		for (long i = it->second - 1; i <= it->second + 1; ++i) {
			for (long j = it->first - 1; j <= it->first + 1; ++j) {
				cellsToExplore.insert({ j, i });
			}
		}
	}

	LifeGrid newGrid(grid->radius, grid->getNumBuckets());
	/*for (long i = grid->bounds.minY; i <= grid->bounds.maxY; ++i) {
		for (long j = grid->bounds.minX; j <= grid->bounds.maxX; ++j) {
			if (grid->aliveNextGen(j, i))
				newGrid.setAlive(j, i , true);
		}
	}*/
	for (auto it = cellsToExplore.begin(); it != cellsToExplore.end(); ++it) {
		if (grid->aliveNextGen(it->first, it->second))
			newGrid.setAlive(it->first, it->second, true);
	}
	*grid = newGrid;
}

void LifeController::outputWorld(ostream& stream) {
	stream << *grid << endl;
}