#pragma once
#include <SFML/Graphics.hpp>


class Hud;

class Tile
{
public:
	enum tileType
	{
		FLOOR,
		WALL,
		STAIRS
	};

	Tile();
	static const float SIZE;
	static void loadTextures(Hud& hud);
	void setType(tileType type);
	void setPosition(float x, float y);
	void draw(sf::RenderWindow& window);
	bool isPassable();
	tileType getType();
	bool hasMonster();
	bool hasPlayer();
	void setPlayerFlag(bool flag);
	void setMonster(int id);
	int getMonster();
	bool isBlocked();
	void visit();
	bool isVisible();
	void resetVisibility();
	bool isDiscovered();
	void setDiscoveredFlag(bool flag);
	bool hasItem();
	int getItem();
	void setItem(int id);

private:
	tileType type;
	static std::vector<sf::Texture*> textures;
	sf::Sprite sprite;
	int monster;
	bool playerFlag;
	bool visible;
	bool discovered;
	int item;
};
