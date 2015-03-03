#pragma once
#include "Menu.h"


class MainMenu : public Menu
{
public:
	MainMenu(int x, int y, int itemHeight, sf::RenderWindow& window);
	void show();
};