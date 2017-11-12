#pragma once

#include <iostream>
#include "lifeGrid.h"

class LifeController {
public:
	LifeController(LifeGrid *gridIn);
	LifeController(LifeGrid *gridIn, std::istream& input);

	void tick();

	void outputWorld(std::ostream& stream);
private:
	LifeGrid *grid;
};