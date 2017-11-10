#include "lifeGrid.h"
#include <regex>
#include <string>
#include <sstream>
#include <limits>

using namespace std;

long strToLong(string str);

/* regex pattern to match input in the form "(x, y)" */
const regex INPUT_PATTERN("\\((-?[0-9]+),\\s*(-?[0-9]+)\\)");

LifeGrid::LifeGrid(istream& input) {
	minRow = maxRow = minCol = maxCol = 0;

	string line;
	getline(input, line);
	while (!input.eof() && line != "") {
		smatch matches;
		if (regex_match(line, matches, INPUT_PATTERN)) {
			setCell(strToLong(matches[1]), strToLong(matches[2]), true);
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

LifeGrid::LifeGrid(LifeGrid& copy) {
	grid = copy.grid;
}

void LifeGrid::setCell(const long row, const long col, const bool alive) {
	if (row < minRow) minRow = row;
	if (row > maxRow) maxRow = row;
	if (col < minCol) minCol = col;
	if (col > maxCol) maxCol = col;

	grid[{ row, col }] = alive;
}

bool LifeGrid::getCell(const long row, const long col) const {
	auto elem = grid.find({ row, col });
	if (elem == grid.end()) return false;
	else return elem->second;
}

void LifeGrid::tick() {
	LifeGrid newGrid(*this);

	for (long i = minRow; i <= maxRow; ++i) {
		for (long j = minCol; j <= maxCol; ++j) {
			int aliveNeighbors = countAliveNeighbors(i, j);
			if (aliveNeighbors == 3 || (aliveNeighbors == 2 && getCell(i, j)))
				newGrid.setCell(i, j, true);
			else
				newGrid.setCell(i, j, false);
		}
	}

	grid = newGrid.grid;
}

int LifeGrid::countAliveNeighbors(const long row, const long col) const {
	int counter = 0;
	for (long i = row - 1; i <= row + 1; ++i) {
		for (long j = col - 1; j <= col + 1; ++j) {
			if (i != row || j != col) {
				if (getCell(i, j))
					++counter;
			}
		}
	}
	return counter;
}

ostream& operator<<(ostream& stream, const LifeGrid& gr) {
	for (long i = gr.minRow; i <= gr.maxRow; ++i) {
		for (long j = gr.minCol; j <= gr.maxCol; ++j) {
			if (gr.getCell(i, j))
				stream << "X ";
			else
				stream << "- ";
		}
		stream << endl;
	}
	return stream;
}