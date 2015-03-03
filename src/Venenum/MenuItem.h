#pragma once
#include <SFML/Graphics.hpp>

class MenuItem
{
public:
	enum MenuResult
	{
		NOTHING,
		NEW_GAME,
		RESUME_GAME,
		SAVE_AND_QUIT,
		QUIT
	};

	MenuItem(std::string label, MenuResult action, int x, int y);
	void draw(sf::RenderWindow& window);
	MenuResult getAction();
	void setColor(sf::Color color);
	bool contains(int x, int y);

private:
	sf::Text text;
	MenuResult action;
};