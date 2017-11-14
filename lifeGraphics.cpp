/**
	File: lifeGraphics.cpp
	Author: Jack Seibert

	Implementation file for LifeGraphics class.
*/

#include "lifeGraphics.h"
#include <iostream>
#include <math.h> // for fmod
#include <stdlib.h> // for rand
#include <iterator> // for advance

using namespace std;

const int DELAY = 1;
const double DELTA_TRANSLATION = 2.5; // in screen units
const double DELTA_ZOOM = 1.01;
const double SMALLEST_LINE_SIZE = 10;
const double AUTOZOOM_CELL_SIZE = 50;

LifeGraphics::LifeGraphics(LifeGrid * const gridIn, const int width, const int height, const int speedIn)
	: grid(gridIn),
	simulationWidth(width), simulationHeight(height),
	cellWidth((double)width / (gridIn->radius * 2 + 1)), cellHeight((double)height / (gridIn->radius * 2 + 1)),
	evolutionSpeed(speedIn) {
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

	window = SDL_CreateWindow("Game of Life", 100, 100, simulationWidth, simulationHeight, SDL_WINDOW_SHOWN);
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
	handleKeys();

	colorAliveCells(grid->getAliveCells());
	size_t gridSize = grid->radius * 2 + 1;
	drawLines(gridSize, gridSize);

	SDL_RenderPresent(renderer);

	SDL_Delay(DELAY);
}

void LifeGraphics::drawLines(size_t numColumns, size_t numRows) {
	if (cellWidth * zoom < SMALLEST_LINE_SIZE)
		numColumns = zoom * simulationWidth / SMALLEST_LINE_SIZE;
	if (cellHeight * zoom < SMALLEST_LINE_SIZE)
		numRows = zoom * simulationHeight / SMALLEST_LINE_SIZE;

	double deltaX = zoom * simulationWidth / numColumns;
	double deltaY = zoom * simulationHeight / numRows;

	// An offset to mark where to start drawing lines in the window
	double offsetX = fmod(translation.first, deltaX);
	double offsetY = fmod(translation.second, deltaY);

	// These for loops draw the lines only within the window, instead of
	// across the entire off-screen grid
	SDL_SetRenderDrawColor(renderer, 0, 192, 255, SDL_ALPHA_OPAQUE);
	for (double x = max(offsetX, translation.first); x - translation.first <= deltaX * numColumns && x <= simulationWidth; x += deltaX) {
		SDL_RenderDrawLine(renderer, x, max(0.0, translation.second), x, min(simulationHeight * zoom + translation.second, (double)simulationHeight));
	}
	for (double y = max(offsetY, translation.second); y - translation.second <= deltaY * numRows && y <= simulationHeight; y += deltaY) {
		SDL_RenderDrawLine(renderer, max(0.0, translation.first), y, min(simulationWidth * zoom + translation.first, (double)simulationWidth), y);
	}
}

windowCoordinate LifeGraphics::scaleCoordinate(const windowCoordinate& coord) const {
	return { coord.first * cellWidth * zoom + translation.first, coord.second * cellHeight * zoom + translation.second };
}

coordinate LifeGraphics::translateCoordinateFromGrid(const coordinate& coord) const {
	return { coord.first - grid->bounds.minX, coord.second - grid->bounds.minY };
}

coordinate LifeGraphics::windowToCell(const int x, const int y) const {
	return { (long)((x - translation.first) / (cellWidth * zoom)) + grid->bounds.minX, (long)((y - translation.second) / (cellHeight * zoom)) + grid->bounds.minY };
}

void LifeGraphics::colorCell(const coordinate& cell) {
	windowCoordinate windowCoord = scaleCoordinate(translateCoordinateFromGrid(cell));

	SDL_Rect rect;
	rect.x = windowCoord.first;
	rect.y = windowCoord.second;
	rect.w = cellWidth * zoom;
	rect.h = cellHeight * zoom;

	SDL_RenderFillRect(renderer, &rect);
}

void LifeGraphics::colorAliveCells(const cellSet& aliveCells) {
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	for_each(aliveCells.begin(), aliveCells.end(), [this](const coordinate& cell) { colorCell(cell); });
}

void LifeGraphics::handleEvent(const SDL_Event& event) {
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			clickedOnCell(event.button.x, event.button.y, true);
		} else if (event.button.button == SDL_BUTTON_RIGHT) {
			clickedOnCell(event.button.x, event.button.y, false);
		}
	} else if (event.type == SDL_MOUSEMOTION) {
		if (event.motion.state & SDL_BUTTON_LMASK) {
			clickedOnCell(event.motion.x, event.motion.y, true);
		} else if (event.motion.state & SDL_BUTTON_RMASK) {
			clickedOnCell(event.motion.x, event.motion.y, false);
		}
	} else if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_SPACE && event.key.repeat == 0)
			paused = !paused;
		else if (event.key.keysym.sym == SDLK_z && event.key.repeat == 0)
			autozoom();
		else if (event.key.keysym.sym == SDLK_LEFT)
			changeSpeed(1);
		else if (event.key.keysym.sym == SDLK_RIGHT)
			changeSpeed(-1);
	}
}

void LifeGraphics::handleKeys() {
	const Uint8 *state = SDL_GetKeyboardState(nullptr);
	if (state[SDL_SCANCODE_UP]) {
		zoomBy(DELTA_ZOOM);
	}
	if (state[SDL_SCANCODE_DOWN]) {
		zoomBy(1 / DELTA_ZOOM);
	}

	if (state[SDL_SCANCODE_W])
		translation.second += DELTA_TRANSLATION;
	if (state[SDL_SCANCODE_A])
		translation.first += DELTA_TRANSLATION;
	if (state[SDL_SCANCODE_S])
		translation.second -= DELTA_TRANSLATION;
	if (state[SDL_SCANCODE_D])
		translation.first -= DELTA_TRANSLATION;
}

void LifeGraphics::clickedOnCell(const int mouseX, const int mouseY, const bool alive) {
	if (mouseY != 0) // to keep from title bar clicks
		grid->setAlive(windowToCell(mouseX, mouseY), alive);
}

void LifeGraphics::zoomBy(const double zoomAmount) {
	zoom *= zoomAmount;
	// Translate the camera to zoom towards center, instead of corner
	translation.first = (translation.first - (double)simulationWidth / 2) * zoomAmount + (double)simulationWidth / 2;
	translation.second = (translation.second - (double)simulationHeight / 2) * zoomAmount + (double)simulationHeight / 2;
}

void LifeGraphics::autozoom() {
	if (grid->getAliveCells().empty()) { // if no alive cells, then just zoom forward
		zoomBy(AUTOZOOM_CELL_SIZE / cellWidth / zoom);
	} else {
		cellSet aliveSet = grid->getAliveCells();
		auto randomIt = aliveSet.begin();
		advance(randomIt, rand() % (aliveSet.size() - 1));
		coordinate cell = *randomIt;

		translation = { 0, 0 };
		zoom = 1;
		zoomBy(AUTOZOOM_CELL_SIZE / cellWidth);
		translation.first -= AUTOZOOM_CELL_SIZE * cell.first;
		translation.second -= AUTOZOOM_CELL_SIZE * cell.second;
	}
}

int LifeGraphics::getSpeed() const {
	return evolutionSpeed;
}

void LifeGraphics::changeSpeed(const int change) {
	evolutionSpeed = max(evolutionSpeed + change, 1);
}