#include "lifeGrid.h"

using namespace std;

LifeGrid::LifeGrid(const size_t radiusIn)
	: radius(radiusIn) {
	bounds.minX = bounds.minY = -radius;
	bounds.maxX = bounds.maxY = radius;
}

LifeGrid::LifeGrid(const size_t radiusIn, const size_t numBuckets)
	: LifeGrid::LifeGrid(radiusIn) {
	aliveSet.rehash(numBuckets);
}

LifeGrid::LifeGrid(const LifeGrid& copy) {
	aliveSet = copy.aliveSet;
}

coordinate LifeGrid::wrapCoordinate(const coordinate cell) const {
	// Begins by translating coordinates to allow for wrapping
	long absoluteMaxX = bounds.maxX - bounds.minX + 1;
	long absoluteMaxY = bounds.maxY - bounds.minY + 1;
	long translatedX = cell.first - bounds.minX;
	long translatedY = cell.second - bounds.minY;

	long wrappedX = (absoluteMaxX + translatedX % absoluteMaxX) % absoluteMaxX + bounds.minX;
	long wrappedY = (absoluteMaxY + translatedY % absoluteMaxY) % absoluteMaxY + bounds.minY;

	return { wrappedX, wrappedY };
}

bool LifeGrid::isAlive(const coordinate cell) const {
	return aliveSet.find(wrapCoordinate(cell)) != aliveSet.end();
}

void LifeGrid::setAlive(const coordinate cell, const bool alive) {
	if (alive)
		aliveSet.insert(wrapCoordinate(cell));
	else
		aliveSet.erase(wrapCoordinate(cell));
}

cellSet LifeGrid::getAliveCells() const {
	return aliveSet;
}

size_t LifeGrid::getNumBuckets() const {
	return aliveSet.bucket_count();
}

int LifeGrid::countAliveNeighbors(const coordinate cell) const {
	int counter = 0;
	for (long i = cell.second - 1; i <= cell.second + 1; ++i) {
		for (long j = cell.first - 1; j <= cell.first + 1; ++j) {
			if (i != cell.second || j != cell.first) {
				if (isAlive({ j, i }))
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
			else if (gr.isAlive({ j, i }))
				stream << "X ";
			else
				stream << "- ";
		}
		stream << endl;
	}
	return stream;
}