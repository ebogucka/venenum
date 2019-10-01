#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "Map.h"
#include "Hud.h"


class Player;
class Hud;
class Map;
class Creature;
class Item;

struct GameContext
{
	sf::RenderWindow* window;
	Hud* hud;
	Player* player;
	Map* map;
	std::unordered_map<int, Creature*> monsters;
	std::unordered_map<int, Item*> items;
	int floor;
};
