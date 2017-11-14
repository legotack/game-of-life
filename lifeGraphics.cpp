#include "lifeGraphics.h"
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <iterator>

using namespace std;

const int DELAY = 1;
const double DELTA_TRANSLATION = 2.5; // in screen units
const double DELTA_ZOOM = 1.01;
const double SMALLEST_CELL_SIZE = 10;
const double AUTOZOOM_CELL_SIZE = 50;

LifeGraphics::LifeGraphics(LifeGrid *gridIn, int width, int height, int speedIn)
	: grid(gridIn), menu(width, height - width),
	windowWidth(width), windowHeight(height),
	simulationWidth(width), simulationHeight(width),
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
	handleKeys();

	size_t gridSize = grid->radius * 2 + 1;
	cellWidth = (double)simulationWidth / gridSize;
	cellHeight = (double)simulationHeight / gridSize;

	colorAliveCells(grid->getAliveCells());
	drawLines(gridSize, gridSize);

	SDL_RenderPresent(renderer);

	SDL_Delay(DELAY);
}

void LifeGraphics::drawLines(size_t numColumns, size_t numRows) {
	if (cellWidth * zoom < SMALLEST_CELL_SIZE)
		numColumns = zoom * simulationWidth / SMALLEST_CELL_SIZE;
	if (cellHeight * zoom < SMALLEST_CELL_SIZE)
		numRows = zoom * simulationHeight / SMALLEST_CELL_SIZE;

	double deltaX = zoom * simulationWidth / numColumns;
	double deltaY = zoom * simulationHeight / numRows;

	double offsetX = fmod(translation.first, deltaX);
	double offsetY = fmod(translation.second, deltaY);

	SDL_SetRenderDrawColor(renderer, 0, 192, 255, SDL_ALPHA_OPAQUE);
	for (double x = max(offsetX, translation.first); x - translation.first <= deltaX * numColumns && x <= simulationWidth; x += deltaX) {
		SDL_RenderDrawLine(renderer, x, max(0.0, translation.second), x, min(simulationHeight * zoom + translation.second, (double)simulationHeight));
	}
	for (double y = max(offsetY, translation.second); y - translation.second <= deltaY * numRows && y <= simulationHeight; y += deltaY) {
		SDL_RenderDrawLine(renderer, max(0.0, translation.first), y, min(simulationWidth * zoom + translation.first, (double)simulationWidth), y);
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

void LifeGraphics::clickedOnCell(const int mouseX, const int mouseY) {
	if (mouseY != 0) // to keep from title bar clicks
		grid->setAlive(windowToCell(mouseX, mouseY), true);
}

void LifeGraphics::zoomBy(const double zoomAmount) {
	zoom *= zoomAmount;
	translation.first = (translation.first - (double)simulationWidth / 2) * zoomAmount + (double)simulationWidth / 2;
	translation.second = (translation.second - (double)simulationHeight / 2) * zoomAmount + (double)simulationHeight / 2;
}

void LifeGraphics::autozoom() {
	if (grid->getAliveCells().empty()) {
		zoomBy(AUTOZOOM_CELL_SIZE / cellWidth / zoom);
	} else {
		setType aliveSet = grid->getAliveCells();
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

int LifeGraphics::getSpeed() {
	return evolutionSpeed;
}

void LifeGraphics::changeSpeed(const int change) {
	evolutionSpeed = max(evolutionSpeed + change, 1);
}