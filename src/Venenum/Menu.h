#pragma once
#include <SFML/Graphics.hpp>
#include "MenuItem.h"

class Menu
{
public:
	Menu(int x, int y, int itemHeight, sf::RenderWindow& window);
	Menu::~Menu();
	virtual void show() = 0;
	void addItem(std::string label, MenuItem::MenuResult action);
	void update();
	MenuItem::MenuResult getMenuResponse();

protected:
	MenuItem::MenuResult handleClick(int x, int y);
	void handleHover(int x, int y);
	std::vector<MenuItem *> items;
	sf::Vector2i position;
	int itemHeight;
	int activeItem;
	sf::RenderWindow& window;
};
