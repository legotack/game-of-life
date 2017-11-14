/**
	Program: Game of Life
	Author: Jack Seibert

	This program implements Conway's Game of Life. This
	file sets up the grid, graphics manager, and
	controller, and runs the mainloop.
*/

#include "lifeController.h"
#include "lifeGraphics.h"
#include <fstream> // for ifstream

using namespace std;

void mainloop(const LifeController& controller, LifeGraphics& graphics);

const size_t WINDOW_WIDTH = 640;
const size_t WINDOW_HEIGHT = 640; // 720 for menu
const int INITIAL_SPEED = 5;

const long DEFAULT_RADIUS = 20;
const string DEFAULT_FILE = "samples/default.txt";

int main(int argc, char *argv[]) {
	long radius;
	if (argc < 2) radius = DEFAULT_RADIUS;
	else radius = stol(string(argv[1]));

	LifeGrid grid(radius);

	LifeGraphics graphics(&grid, WINDOW_WIDTH, WINDOW_HEIGHT, INITIAL_SPEED);

	string filename = DEFAULT_FILE;
	if (argc >= 3) {
		filename = string(argv[2]);
	}

	ifstream stream(filename);
	LifeController controller(&grid, stream);
	stream.close();
	
	mainloop(controller, graphics);

	return 0;
}

void mainloop(const LifeController& controller, LifeGraphics& graphics) {
	int sinceLastGeneration = 0;
	while (!graphics.quit) {
		if (!graphics.paused) ++sinceLastGeneration;
		graphics.tick();
		if (sinceLastGeneration >= graphics.getSpeed()) {
			controller.nextGeneration();
			sinceLastGeneration = 0;
		}
	}
}