#pragma once
#include <SFML/Graphics.hpp>
#include <fstream>
#include "Tile.h"
#include "GameContext.h"
#include "Hud.h"
#include "Creature.h"


class Tile;
class Hud;
struct GameContext;

class Map
{
public:
	Map(GameContext& context);
	~Map(void);
	static const int MAP_WIDTH = 100;
	static const int MAP_HEIGHT = 100;
	static const int ROOM_MAX_SIZE = 12;
	static const int ROOM_MIN_SIZE = 8;
	static const int MAX_ROOMS = 30;
	void generate();
	void createRoom(sf::IntRect room);
	void createHorizontalCorridor(int x1, int x2, int y);
	void createVerticalCorridor(int y1, int y2, int x);
	void draw();
	bool isTilePassable(int x, int y);
	void load();
	void save();
	Tile::tileType getTileType(int x, int y);
	bool hasMonster(int x, int y);
	bool hasPlayer(int x, int y);
	void setPlayerFlag(int x, int y, bool flag);
	void resetMonster(int x, int y);
	void setMonster(int x, int y, int id);
	int getMonster(int x, int y);
	bool isVisible(int x, int y);
	void resetVisibility();
	bool isBlocked(short x, short y);
	void visit(short x, short y);
	bool hasItem(int x, int y);
	int getItem(int x, int y);
	void setItem(int x, int y, int id);
	void removeItem(int x, int y);

private:
	GameContext& context;
	int lastLevel;
	Tile** tiles;

	class Room : public sf::IntRect
	{
	public:
		Room(int left, int top, int width, int height) : sf::IntRect(left, top, width, height) {}
		sf::Vector2i getCenter()
		{
			sf::Vector2i center;
			center.x = (2 * left + width) / 2;
			center.y = (2 * top + height) / 2;
			return center;
		}
	};

	void setType(int x, int y, Tile::tileType type);
};

