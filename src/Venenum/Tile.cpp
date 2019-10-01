#include "Tile.h"
#include "Hud.h"


Tile::Tile() : type(WALL), monster(-1), item(-1), playerFlag(false), visible(false), discovered(false) {}


void Tile::loadTextures(Hud& hud)
{
	textures.push_back(new sf::Texture());
	textures[Tile::FLOOR]->loadFromFile("../images/tiles/floor.png");

	textures.push_back(new sf::Texture());
	textures[Tile::WALL]->loadFromFile("../images/tiles/wall.png");

	textures.push_back(new sf::Texture());
	textures[Tile::STAIRS]->loadFromFile("../images/tiles/stairs.png");
}


void Tile::setType(Tile::tileType type)
{
	this->type = type;
	sprite.setTexture(*textures[type]);
	sprite.setColor(sf::Color(50, 50, 50));
}


void Tile::setPosition(float x, float y)
{
	sprite.setPosition(x, y);
}


void Tile::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}


bool Tile::isPassable()
{
	return (!isBlocked() && type != Tile::STAIRS && !hasMonster() && !hasPlayer());
}


Tile::tileType Tile::getType()
{
	return type;
}


bool Tile::hasMonster()
{
	return monster != -1;
}


bool Tile::hasPlayer()
{
	return playerFlag;
}


void Tile::setPlayerFlag(bool flag)
{
	playerFlag = flag;
}


void Tile::setMonster(int id)
{
	monster = id;
}


int Tile::getMonster()
{
	return monster;
}


bool Tile::isBlocked()
{
	return (type != Tile::FLOOR && type != Tile::STAIRS);
}
	

void Tile::visit()
{
	visible = true;
	discovered = true;
	sprite.setColor(sf::Color(255, 255, 255));
}


bool Tile::isVisible()
{
	return visible;
}


void Tile::resetVisibility()
{
	visible = false;
	sprite.setColor(sf::Color(50, 50, 50));
}


bool Tile::isDiscovered()
{
	return discovered;
}


void Tile::setDiscoveredFlag(bool flag)
{
	discovered = flag;
}


bool Tile::hasItem()
{
	return item != -1;
}


int Tile::getItem()
{
	return item;
}


void Tile::setItem(int id)
{
	item = id;
}

const float Tile::SIZE = 32.0f;
std::vector<sf::Texture*> Tile::textures;