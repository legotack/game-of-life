#include <iostream>
#include "lifeController.h"
#include "lifeGraphics.h"
#include "lifeSamples.h"
#include <SDL2/SDL.h>
#include <sstream>

using namespace std;

const size_t WINDOW_WIDTH = 640;
const size_t WINDOW_HEIGHT = 640;
const int SPEED = 100;

const string SAMPLE_INPUT = "(1, 0)\n(2, 1)\n(2, 2)\n(1, 2)\n(0, 2)\n(-2000000000002, -2000000000000)\n(-2000000000001, -2000000000000)\n(-2000000000000, -2000000000000)\n";

int main() {
	LifeGrid grid(20);

	LifeGraphics graphics(&grid, WINDOW_WIDTH, WINDOW_HEIGHT);

	stringstream stream(LifeSamples::samples["Glider"]);
	LifeController controller(&grid, stream);

	while (!graphics.quit) {
		//controller.outputWorld(cout);
		graphics.tick();
		// experiment with order of these
		if (!graphics.paused) controller.tick();
		SDL_Delay(SPEED);
	}

	return 0;
}