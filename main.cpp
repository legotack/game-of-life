#include <iostream>
#include "lifeGrid.h"
#include <SDL2/SDL.h>

using namespace std;

int main() {
	LifeGrid grid(cin, 5);

	while (true) {
		cout << grid << endl;
		grid.tick();
		SDL_Delay(500);
	}

	return 0;
}