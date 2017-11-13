#include <iostream>
#include "lifeController.h"
#include "lifeGraphics.h"
#include "lifeSamples.h"
#include <sstream>
#include <fstream>

using namespace std;

const size_t WINDOW_WIDTH = 640;
const size_t WINDOW_HEIGHT = 760;

const string SAMPLE_INPUT = "(1, 0)\n(2, 1)\n(2, 2)\n(1, 2)\n(0, 2)\n(-2000000000002, -2000000000000)\n(-2000000000001, -2000000000000)\n(-2000000000000, -2000000000000)\n";

int main(int argc, char *argv[]) {
	LifeGrid grid(20);

	LifeGraphics graphics(&grid, WINDOW_WIDTH, WINDOW_HEIGHT);

	LifeController controller;
	if (argc < 2) {
		stringstream stream(LifeSamples::samples["Glider"]);
		controller = LifeController(&grid, stream);
	} else {
		ifstream stream((string(argv[1])));
		controller = LifeController(&grid, stream);
	}

	

	while (!graphics.quit) {
		//controller.outputWorld(cout);
		graphics.tick();
		// experiment with order of these
		if (!graphics.paused) controller.tick();
	}

	return 0;
}