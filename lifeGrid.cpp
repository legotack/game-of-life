#include "lifeGrid.h"
#include <regex>
#include <string>
#include <sstream>
#include <limits>

using namespace std;

long strToLong(string str);

/* regex pattern to match input in the form "(x, y)" */
const regex INPUT_PATTERN("\\((-?[0-9]+),\\s*(-?[0-9]+)\\)");

LifeGrid::LifeGrid(istream& input, size_t radius) {
	bounds.minX = bounds.minY = -radius;
	bounds.maxX = bounds.maxY = radius;

	string line;
	getline(input, line);
	while (!input.eof() && line != "") {
		smatch matches;
		if (regex_match(line, matches, INPUT_PATTERN)) {
			setAlive(strToLong(matches[1]), strToLong(matches[2]), true);
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
	aliveSet = copy.aliveSet;
}

// make better
coordinate LifeGrid::wrapCoordinate(const long x, const long y) const {
	long wrappedX, wrappedY;
	if (x > bounds.maxX) {
		wrappedX = (x - bounds.maxX - 1) + bounds.minX;
	} else if (x < bounds.minX) {
		wrappedX = (x - bounds.minX + 1) + bounds.maxX;
	} else {
		wrappedX = x;
	}

	if (y > bounds.maxY) {
		wrappedY = (y - bounds.maxY - 1) + bounds.minY;
	} else if (y < bounds.minY) {
		wrappedY = (y - bounds.minY + 1) + bounds.maxY;
	} else {
		wrappedY = y;
	}

	return { wrappedX, wrappedY };
}

bool LifeGrid::isAlive(const long x, const long y) const {
	return aliveSet.find(wrapCoordinate(x, y)) != aliveSet.end();
}

void LifeGrid::setAlive(const long x, const long y, const bool alive) {
	if (alive)
		aliveSet.insert(wrapCoordinate(x, y));
	else
		aliveSet.erase(wrapCoordinate(x, y));
}

// see if you can do this smarter, to only look near alive cells
void LifeGrid::tick() {
	setType newSet(aliveSet.bucket_count());
	for (long i = bounds.minY; i <= bounds.maxY; ++i) {
		for (long j = bounds.minX; j <= bounds.maxX; ++j) {
			int aliveNeighbors = countAliveNeighbors(j, i);
			if (aliveNeighbors == 3 || (aliveNeighbors == 2 && isAlive(j, i)))
				newSet.insert({ j, i }); // to avoid this, make setType into a class (with better name)
		}
	}
	aliveSet = newSet;
}

int LifeGrid::countAliveNeighbors(const long x, const long y) const {
	int counter = 0;
	for (long i = y - 1; i <= y + 1; ++i) {
		for (long j = x - 1; j <= x + 1; ++j) {
			if (i != y || j != x) {
				if (isAlive(j, i))
					++counter;
			}
		}
	}
	return counter;
}

ostream& operator<<(ostream& stream, const LifeGrid& gr) {
	for (long i = gr.bounds.minY; i <= gr.bounds.maxY; ++i) {
		for (long j = gr.bounds.minX; j <= gr.bounds.maxX; ++j) {
			if (i == 0 && j == 0)
				stream << "O "; // marks the center
			else if (gr.isAlive(j, i))
				stream << "X ";
			else
				stream << "- ";
		}
		stream << endl;
	}
	return stream;
}