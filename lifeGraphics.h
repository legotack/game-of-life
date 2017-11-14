/**
	File: lifeGraphics.h
	Author: Jack Seibert

	LifeGraphics class handles all graphics and events for the
	GUI window. tick() should be called from a mainloop.
*/

#pragma once

#include "lifeGrid.h"
#include <SDL2/SDL.h>

// type for handling floating point coordinates for the window
using windowCoordinate = std::pair<double, double>;

class LifeGraphics {
public:
	/* Constructor/Destructor:
	   -----------------------
	   Sets up all constant member variables, and loads SDL window. Throws an error
	   if SDL fails. Destructor cleans up SDL.
	*/
	LifeGraphics(LifeGrid * const gridIn, const int width, const int height, const int speedIn);
	~LifeGraphics();

	/* Function: tick
	   --------------
	   Should be called from within a mainloop. Runs one frame of the GUI, including a delay
	   at the end. Draws cells, grid lines, and handles mouse and keyboard events.
	*/
	void tick();

	/* Function: getSpeed
	   ------------------
	   Returns the speed at which the simulation should be running, which is controlled
	   by the user, with the left and right arrow keys.
	*/
	int getSpeed() const;

	bool paused; // true if the simulation should be paused, false otherwise.
	bool quit; // true when the SDL window is closed, and the program should stop running.
private:
	LifeGrid * const grid; // pointer to the LifeGrid object, which contains all the information on cells.

	// SDL objects:
	SDL_Window *window;
	SDL_Renderer *renderer;

	// Width and height of the simulation window.
	const int simulationWidth;
	const int simulationHeight;

	// Width and height of a single cell in pixels, at a zoom of 1. These
	// values do not change, even though the drawn cell width and height
	// will change as the user zooms in and out.
	const double cellWidth;
	const double cellHeight;

	// A pair holding the translation of the camera. Expressed in terms of
	// how the objects rendered move; e.g. a negative translation moves the grid
	// to the left.
	windowCoordinate translation;
	// A double representing how zoomed-in the camera is. Begins at 1, and higher
	// numbers result in a bigger image, while lower numbers result in a smaller
	// image.
	double zoom;

	// The speed at which the simulation should be running, expressed in terms of
	// the number of frames to wait between each evolution.
	int evolutionSpeed;

	/* Function: loadSDL
	   -----------------
	   Loads SDL and creates the window and renderer.
	*/
	int loadSDL();

	/* Function: scaleCoordinate
	   -------------------------
	   Given a window coordinate, scales it based on the current zoom and translation.
	*/
	windowCoordinate scaleCoordinate(const windowCoordinate& coord) const;

	/* Function: translateCoordinateFromGrid
	   -------------------------------------
	   Given a grid coordinate, shifts it in relation to an origin at the top-left
	   of the screen instead of at the center.
	*/
	coordinate translateCoordinateFromGrid(const coordinate& coord) const;
	/* Function: windowToCell
	   ----------------------
	   Given an x and y position in the window, converts it to the coordinate
	   of a cell in the LifeGrid.
	*/
	coordinate windowToCell(const int x, const int y) const;

	/* Function: drawLines
	   -------------------
	   Draws the grid lines necessary for the current camera view.
	*/
	void drawLines(size_t numColumns, size_t numRows);
	/* Function colorAliveCells
	   ------------------------
	   Given a set of all alive cells, colors them in the graphics window.
	*/
	void colorAliveCells(const cellSet& aliveCells);

	/* Function: colorCell
	   -------------------
	   Given a grid coordinate of a cell, colors it in the graphics window.
	*/
	void colorCell(const coordinate& cell);

	/* Functions: handleEvent, handleKeys
	   ----------------------------------
	   Given an SDL_Event object, handles whatever event is in it, calling the
	   appropriate event handlers. handleKeys handles keyboard input.
	*/
	void handleEvent(const SDL_Event& event);
	void handleKeys();

	/* Function: clickedOnCell
	   -----------------------
	   Given a mouse position, does the appropriate action for that cell, governed
	   by the alive parameter (i.e. either makes it alive or makes it dead).
	*/
	void clickedOnCell(const int mouseX, const int mouseY, const bool alive);

	/* Function: zoomBy
	   ----------------
	   Zooms the camera by zoomAmount.
	*/
	void zoomBy(const double zoomAmount);
	/* Function: autozoom
	   ------------------
	   Automatically zooms and translates the camera to focus on a random alive cell.
	*/
	void autozoom();

	/* Function: changeSpeed
	   ---------------------
	   Changes evolutionSpeed by the amount given by change. Keeps speed at minimum 1,
	   because any lower has no effect.
	*/
	void changeSpeed(const int change);
};