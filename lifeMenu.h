#pragma once

#include <SDL2/SDL.h>

class LifeMenu {
public:
	LifeMenu(const int widthIn, const int heightIn);

	void handleClick(const int mouseX, const int mouseY);
private:
	const int menuWidth;
	const int menuHeight;
};