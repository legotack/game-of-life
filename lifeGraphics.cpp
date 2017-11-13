#include "lifeGraphics.h"
#include <iostream>

using namespace std;

const int SPEED = 100;
const double DELTA_TRANSLATION = 20; // in screen units
const double DELTA_ZOOM = 0.2;

LifeGraphics::LifeGraphics(LifeGrid *gridIn, int width, int height)
	: grid(gridIn), menu(width, height - width),
	windowWidth(width), windowHeight(height),
	simulationWidth(width), simulationHeight(width) {
	zoom = 1;
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
	cellWidth = (double)simulationWidth / gridSize;
	cellHeight = (double)simulationHeight / gridSize;

	colorAliveCells(grid->getAliveCells());
	drawLines(gridSize, gridSize);

	SDL_RenderPresent(renderer);

	SDL_Delay(SPEED);
}

void LifeGraphics::drawLines(size_t numColumns, size_t numRows) {
	if (numColumns > 50) numColumns = 20;
	if (numRows > 50) numRows = 20;

	double deltaX = zoom * simulationWidth / numColumns;
	double deltaY = zoom * simulationHeight / numRows;

	SDL_SetRenderDrawColor(renderer, 0, 192, 255, SDL_ALPHA_OPAQUE);
	for (int x = 0; x <= numColumns; ++x) {
		SDL_RenderDrawLine(renderer, x * deltaX + translation.first, translation.second, x * deltaX + translation.first, simulationHeight * zoom + translation.second);
	}
	for (int y = 0; y <= numRows; ++y) {
		SDL_RenderDrawLine(renderer, translation.first, y * deltaY + translation.second, simulationWidth * zoom + translation.first, y * deltaY + translation.second);
	}
}

windowCoordinate LifeGraphics::scaleCoordinate(const windowCoordinate& coord) {
	return { coord.first * cellWidth * zoom + translation.first, coord.second * cellHeight * zoom + translation.second };
}

// translates from -+ to all +
coordinate LifeGraphics::translateCoordinateFromGrid(const coordinate& coord) {
	return { coord.first - grid->bounds.minX, coord.second - grid->bounds.minY };
}

coordinate LifeGraphics::windowToCell(const int x, const int y) {
	return { (long)((x - translation.first) / (cellWidth * zoom)) + grid->bounds.minX, (long)((y - translation.second) / (cellHeight * zoom)) + grid->bounds.minY };
}

// cell is in LifeGrid coordinates
void LifeGraphics::colorCell(const coordinate& cell) {
	windowCoordinate windowCoord = scaleCoordinate(translateCoordinateFromGrid(cell));

	SDL_Rect rect;
	rect.x = windowCoord.first;
	rect.y = windowCoord.second;
	rect.w = cellWidth * zoom;
	rect.h = cellHeight * zoom;

	SDL_RenderFillRect(renderer, &rect);
}

void LifeGraphics::colorAliveCells(const setType& aliveCells) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	for_each(aliveCells.begin(), aliveCells.end(), [this](const coordinate& cell) { colorCell(cell); });
}

void LifeGraphics::handleEvent(const SDL_Event& event) {
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			if (event.button.y < simulationHeight)
				clickedOnCell(event.button.x, event.button.y);
			else
				menu.handleClick(event.button.x, event.button.y);
		}
	} else if (event.type == SDL_MOUSEMOTION) {
		if (event.motion.state & SDL_BUTTON_LMASK) {
			if (event.motion.y < simulationHeight)
				clickedOnCell(event.motion.x, event.motion.y);
			else
				menu.handleClick(event.motion.x, event.motion.y);
		}
	} else if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
		switch (event.key.keysym.sym) {
			case SDLK_SPACE:
				paused = !paused;
				break;
			case SDLK_UP:
				zoom += DELTA_ZOOM;
				break;
			case SDLK_DOWN:
				zoom -= DELTA_ZOOM;
				break;
			case SDLK_w:
				translation.second += DELTA_TRANSLATION;
				break;
			case SDLK_a:
				translation.first += DELTA_TRANSLATION;
				break;
			case SDLK_s:
				translation.second -= DELTA_TRANSLATION;
				break;
			case SDLK_d:
				translation.first -= DELTA_TRANSLATION;
				break;
		}
	}
}

void LifeGraphics::clickedOnCell(const int mouseX, const int mouseY) {
	if (mouseY != 0) // to keep from title bar clicks
		grid->setAlive(windowToCell(mouseX, mouseY), true);
}