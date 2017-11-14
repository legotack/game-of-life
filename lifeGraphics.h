#pragma once

#include "lifeGrid.h"
#include "lifeMenu.h"
#include <SDL2/SDL.h>

using windowCoordinate = std::pair<double, double>;

class LifeGraphics {
public:
	LifeGraphics(LifeGrid *gridIn, int width, int height, int speedIn);
	~LifeGraphics();

	void tick();

	int getSpeed();

	bool paused;
	bool quit;
private:
	LifeGrid *grid;
	LifeMenu menu;

	SDL_Window *window;
	SDL_Renderer *renderer;

	int windowWidth;
	int windowHeight;

	int simulationWidth;
	int simulationHeight;

	double cellWidth;
	double cellHeight;

	windowCoordinate translation;
	double zoom;

	int evolutionSpeed;

	int loadSDL();

	windowCoordinate scaleCoordinate(const windowCoordinate& coord);

	coordinate translateCoordinateFromGrid(const coordinate& coord);
	coordinate windowToCell(const int x, const int y);

	void colorCell(const coordinate& cell);

	void drawLines(size_t numColumns, size_t numRows);
	void colorAliveCells(const setType& aliveCells);

	void handleEvent(const SDL_Event& event);
	void handleKeys();
	void clickedOnCell(const int mouseX, const int mouseY, const bool alive);

	void zoomBy(const double zoomAmount);
	void autozoom();

	void changeSpeed(const int change);
};