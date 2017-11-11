/**
	Defines the LifeCell type for modelling one
	cell in Conway's Game of Life. Each cell
	has a row and column, and has pointers to
	the eight cells around it.

	Author: Jack Seibert
	Date: 11/10/17
*/

class LifeCell {
public:
	LifeCell(long row, long col);

	bool isAlive() const;
	void setAlive(bool isAlive);

	int countNeighbors() const;
private:
	long row, col;
	bool alive;
	LifeCell *neighbors[8]; // starts in upper left corner, goes clockwise
};