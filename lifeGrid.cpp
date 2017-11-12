#include "lifeGrid.h"

using namespace std;

LifeGrid::LifeGrid(size_t radiusIn)
	: radius(radiusIn) {
	bounds.minX = bounds.minY = -radius;
	bounds.maxX = bounds.maxY = radius;
}

LifeGrid::LifeGrid(size_t radiusIn, size_t numBuckets)
	: LifeGrid::LifeGrid(radiusIn) {
	aliveSet.rehash(numBuckets);
}

LifeGrid::LifeGrid(LifeGrid& copy) {
	aliveSet = copy.aliveSet;
}

// make better
coordinate LifeGrid::wrapCoordinate(const long x, const long y) const {
	long absoluteMaxX = bounds.maxX - bounds.minX + 1;
	long absoluteMaxY = bounds.maxY - bounds.minY + 1;
	long translatedX = x - bounds.minX;
	long translatedY = y - bounds.minY;

	long wrappedX = (absoluteMaxX + translatedX % absoluteMaxX) % absoluteMaxX + bounds.minX;
	long wrappedY = (absoluteMaxY + translatedY % absoluteMaxY) % absoluteMaxY + bounds.minY;

	return { wrappedX, wrappedY };
}

bool LifeGrid::isAlive(const long x, const long y) const {
	return aliveSet.find(wrapCoordinate(x, y)) != aliveSet.end();
}

void LifeGrid::setAlive(const coordinate cell, const bool alive) {
	if (alive)
		aliveSet.insert(wrapCoordinate(cell.first, cell.second));
	else
		aliveSet.erase(wrapCoordinate(cell.first, cell.second));
}

void LifeGrid::setAlive(const long x, const long y, const bool alive) {
	setAlive({ x, y }, alive);
}

setType LifeGrid::getAliveCells() {
	return aliveSet;
}

size_t LifeGrid::getNumBuckets() {
	return aliveSet.bucket_count();
}

bool LifeGrid::aliveNextGen(const long x, const long y) const {
	int aliveNeighbors = countAliveNeighbors(x, y);
	return aliveNeighbors == 3 || (aliveNeighbors == 2 && isAlive(x, y));
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