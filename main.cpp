#include <iostream>
#include "lifeController.h"
#include "lifeGraphics.h"
#include "lifeSamples.h"
#include <sstream>
#include <fstream>

using namespace std;

const size_t WINDOW_WIDTH = 640;
const size_t WINDOW_HEIGHT = 640; // 720 for menu
const int INITIAL_SPEED = 5;

const long DEFAULT_RADIUS = 20;

int main(int argc, char *argv[]) {
	long radius;
	if (argc < 2) radius = DEFAULT_RADIUS;
	else radius = stol(string(argv[1]));

	LifeGrid grid(radius);

	LifeGraphics graphics(&grid, WINDOW_WIDTH, WINDOW_HEIGHT, INITIAL_SPEED);

	LifeController controller;
	if (argc < 3) {
		stringstream stream(LifeSamples::samples["Glider"]);
		controller = LifeController(&grid, stream);
	} else {
		ifstream stream((string(argv[2])));
		controller = LifeController(&grid, stream);
	}

	
	int sinceLastGeneration = 0;
	while (!graphics.quit) {
		if (!graphics.paused)++sinceLastGeneration;
		//controller.outputWorld(cout);
		graphics.tick();
		// experiment with order of these
		if (sinceLastGeneration >= graphics.getSpeed()) {
			controller.tick();
			sinceLastGeneration = 0;
		}
	}

	return 0;
}