#include "lifeGraphics.h"
#include <iostream>

using namespace std;

LifeGraphics::LifeGraphics(LifeGrid *gridIn, size_t width, size_t height)
	: grid(gridIn), windowWidth(width), windowHeight(height) {
	quit = false;
	paused = false;
	if (loadSDL() != 0)
		throw "Error setting up SDL";
}

LifeGraphics::~LifeGraphics() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int LifeGraphics::loadSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		cout << "SDL_Init Error: " << SDL_GetError() << endl;
		return 1;
	}

	window = SDL_CreateWindow("Game of Life", 100, 100, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		cout << "SDL_CreateWindow Error: " << SDL_GetError() << endl;
		SDL_Quit();
		return 1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		SDL_DestroyWindow(window);
		cout << "SDL_CreateRenderer Error: " << SDL_GetError() << endl;
		SDL_Quit();
		return 1;
	}

	return 0;
}

void LifeGraphics::tick() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) quit = true;
		handleEvent(event);
	}

	size_t gridSize = grid->radius * 2 + 1;
	cellWidth = (double)windowWidth / gridSize;
	cellHeight = (double)windowHeight / gridSize;

	colorAliveCells(grid->getAliveCells());
	drawLines(gridSize, gridSize);

	SDL_RenderPresent(renderer);
}

void LifeGraphics::drawLines(size_t numColumns, size_t numRows) {
	if (numColumns > 50) numColumns = 20;
	if (numRows > 50) numRows = 20;

	double deltaX = (double)windowWidth / numColumns;
	double deltaY = (double)windowHeight / numRows;

	SDL_SetRenderDrawColor(renderer, 0, 192, 255, SDL_ALPHA_OPAQUE);
	for (int x = 1; x < numColumns; ++x) {
		SDL_RenderDrawLine(renderer, x * deltaX, 0, x * deltaX, windowHeight);
	}
	for (int y = 1; y < numRows; ++y) {
		SDL_RenderDrawLine(renderer, 0, y * deltaY, windowWidth, y * deltaY);
	}
}

coordinate LifeGraphics::translateCoordinate(const coordinate& coord) {
	return { coord.first - grid->bounds.minX, coord.second - grid->bounds.minY };
}

coordinate LifeGraphics::windowToCell(const int x, const int y) {
	return { (long)(x / cellWidth) + grid->bounds.minX, (long)(y / cellHeight) + grid->bounds.minY };
}

void LifeGraphics::colorCell(const coordinate& cell) {
	SDL_Rect rect;
	rect.x = cell.first * cellWidth;
	rect.y = cell.second * cellHeight;
	rect.w = cellWidth;
	rect.h = cellHeight;

	SDL_RenderFillRect(renderer, &rect);
}

void LifeGraphics::colorAliveCells(setType aliveCells) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	for_each(aliveCells.begin(), aliveCells.end(), [this](const coordinate& cell) { colorCell(translateCoordinate(cell)); });
}

void LifeGraphics::handleEvent(SDL_Event& event) {
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			grid->setAlive(windowToCell(event.motion.x, event.motion.y), true);
		}
	} else if (event.type == SDL_MOUSEMOTION) {
		if (event.motion.state & SDL_BUTTON_LMASK) {
			grid->setAlive(windowToCell(event.motion.x, event.motion.y), true);
		}
	} else if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
		switch (event.key.keysym.sym) {
			case SDLK_SPACE:
				paused = !paused;
				break;
		}
	}
}