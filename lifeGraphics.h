#pragma once

#include "lifeGrid.h"
#include <SDL2/SDL.h>

class LifeGraphics {
public:
	LifeGraphics(LifeGrid *gridIn, size_t width, size_t height);
	~LifeGraphics();

	void tick();

	bool paused;
	bool quit;
private:
	LifeGrid *grid;

	SDL_Window *window;
	SDL_Renderer *renderer;

	size_t windowWidth;
	size_t windowHeight;

	double cellWidth;
	double cellHeight;

	int loadSDL();

	coordinate translateCoordinate(const coordinate& coord);
	coordinate windowToCell(const int x, const int y);

	void colorCell(const coordinate& cell);

	void drawLines(size_t numColumns, size_t numRows);
	void colorAliveCells(setType aliveCells);

	void handleEvent(SDL_Event& event);
};