/**
	Defines the LifeGrid type for handling
	Conway's Game of Life. Backed by a
	unordered_map in order to support large, negative,
	sparse values.

	Author: Jack Seibert
	Date: 11/9/17
*/

#pragma once

#include <iostream>
#include <unordered_map>

class LifeGrid {
public:
	LifeGrid(std::istream& input);

	void tick();
private:
	std::unordered_map<long, bool> grid;
};