#include "Map.h"
#include "Player.h"
#include "Item.h"

Map::Map(GameContext& context) : context(context)
{
	tiles = new Tile*[MAP_WIDTH];
	for(int i = 0; i < MAP_HEIGHT; i++)
	{
		tiles[i] = new Tile[MAP_HEIGHT];
	}
	for(int i = 0; i < MAP_WIDTH; i++)
	{
		for(int j = 0; j < MAP_HEIGHT; j++)
		{
			tiles[i][j].setPosition(i * Tile::SIZE, j * Tile::SIZE);
		}
	}
}


/* Based on http://roguebasin.roguelikedevelopment.org/index.php?
   title=Complete_Roguelike_Tutorial,_using_python%2Blibtcod,_part_3&oldid=30347 */

void Map::generate()
{
	for(int x = 0; x < MAP_WIDTH; x++)
	{
		for(int y = 0; y < MAP_HEIGHT; y++)
		{
			tiles[x][y].setType(Tile::WALL);
			tiles[x][y].setDiscoveredFlag(false);
			tiles[x][y].setItem(-1);
			tiles[x][y].setPlayerFlag(false);
			tiles[x][y].setMonster(-1);
		}
	}

	std::vector<Room> rooms;
	
	for(int i = 0; i < MAX_ROOMS; i++)
	{
		int roomWidth = rand() % ROOM_MAX_SIZE + ROOM_MIN_SIZE;
		int roomHeight = rand() % ROOM_MAX_SIZE + ROOM_MIN_SIZE; 
		int roomX = rand() % (MAP_WIDTH - roomWidth - 1);
        int roomY = rand() % (MAP_HEIGHT - roomHeight - 1);
       
		Room newRoom = Room(roomX, roomY, roomWidth, roomHeight);

        bool failed = false;
		for(int j = 0; j < rooms.size(); j++)
		{
			if(newRoom.intersects(rooms[j]))
			{
				failed = true;
				break;
			}
		}

		if(!failed)
		{
			createRoom(newRoom);
			sf::Vector2i newRoomCenter = newRoom.getCenter();
			if(i == 0)
			{
				context.player->move(newRoomCenter.x, newRoomCenter.y);
			}
			else
			{
				sf::Vector2i previousRoomCenter = rooms[rooms.size()-1].getCenter();
				if(rand() % 1 == 1)
				{
					createHorizontalCorridor(previousRoomCenter.x, newRoomCenter.x, previousRoomCenter.y);
                    createVerticalCorridor(previousRoomCenter.y, newRoomCenter.y, newRoomCenter.x);
				}
                else
				{
                    createVerticalCorridor(previousRoomCenter.y, newRoomCenter.y, previousRoomCenter.x);
					createHorizontalCorridor(previousRoomCenter.x, newRoomCenter.x, newRoomCenter.y);
				}
			}
            rooms.push_back(newRoom);
		}
	}
	if(context.floor < Game::LAST_FLOOR)
	{
		setType(rooms.back().getCenter().x, rooms.back().getCenter().y, Tile::STAIRS);
	}
	else
	{
		setItem(rooms.back().getCenter().x, rooms.back().getCenter().y, context.items.size()-1);
	}
}


void Map::createRoom(sf::IntRect room)
{
	for(int x = room.left + 1; x < room.left + room.width; x++)
	{
		for(int y = room.top + 1; y < room.top + room.height; y++)
		{
			tiles[x][y].setType(Tile::FLOOR);
		}
	}
}


void Map::createHorizontalCorridor(int x1, int x2, int y)
{
	for(int x = std::min(x1, x2); x < std::max(x1, x2) + 1; x++)
	{
		tiles[x][y].setType(Tile::FLOOR);
	}
}


void Map::createVerticalCorridor(int y1, int y2, int x)
{
	for(int y = std::min(y1, y2); y < std::max(y1, y2) + 1; y++)
	{
		tiles[x][y].setType(Tile::FLOOR);
	}
}


void Map::draw()
{
	for(int i = 0; i < MAP_WIDTH; i++)
	{
		for(int j = 0; j < MAP_HEIGHT; j++)
		{
			if(tiles[i][j].isDiscovered())
			{
				tiles[i][j].draw(*context.window);
			}
			if(tiles[i][j].isVisible())
			{
				if(tiles[i][j].hasItem())
				{
					context.items[tiles[i][j].getItem()]->draw();
				}
				if(tiles[i][j].hasMonster())
				{
					context.monsters[tiles[i][j].getMonster()]->draw();
				}
			}
		}
	}
}


void Map::resetVisibility()
{
	for(int i = 0; i < MAP_WIDTH; i++)
	{
		for(int j = 0; j < MAP_HEIGHT; j++)
		{
			tiles[i][j].resetVisibility();
		}
	}
	for(std::unordered_map<int, Creature *>::iterator it = context.monsters.begin(); it != context.monsters.end(); ++it)
	{
		it->second->setPlayerVisibility(false);
	}
}


bool Map::isTilePassable(int x, int y)
{
	return tiles[x][y].isPassable();
}


void Map::load()
{
	std::ifstream mapFile("data/map.dat");
	std::ifstream discoveredMapFile("data/map2.dat");

	if(!mapFile)
	{
		std::ostringstream msg;
		msg << "Failed to load map from data/map.dat!";
		context.hud->writeMessage(msg);
	}
	if(!discoveredMapFile)
	{
		std::ostringstream msg;
		msg << "Failed to load map from data/map2.dat!";
		context.hud->writeMessage(msg);
	}
	if(mapFile.is_open()&& discoveredMapFile.is_open())
	{
		char c, d;
		for(int i = 0; i < MAP_HEIGHT; i++)
		{
			for(int j = 0; j < MAP_WIDTH; j++)
			{
				c = mapFile.get();
				d = discoveredMapFile.get();
				switch(c)
				{
				case '.':
					tiles[j][i].setType(Tile::FLOOR);
					break;
				case '#':
					tiles[j][i].setType(Tile::WALL);
					break;
				case 'S':
					tiles[j][i].setType(Tile::STAIRS);
					break;
				case '\n':
					break;
				}

				switch(d)
				{
				case '.':
					tiles[j][i].setDiscoveredFlag(false);
					break;
				case '*':
					tiles[j][i].setDiscoveredFlag(true);
					break;
				case '\n':
					j--;
					break;
				}
			}
		}
		mapFile.close();
		discoveredMapFile.close();
	}
}


void Map::save()
{
	std::ofstream mapFile("data/map.dat");
	std::ofstream discoveredMapFile("data/map2.dat");
	
	if(mapFile.is_open() && discoveredMapFile.is_open())
	{
		for(int i = 0; i < MAP_HEIGHT; i++)
		{
			for(int j = 0; j < MAP_WIDTH; j++)
			{
				switch(tiles[j][i].getType())
				{
				case Tile::FLOOR:
					mapFile.put('.');
					break;
				case Tile::WALL:
					mapFile.put('#');
					break;
				case Tile::STAIRS:
					mapFile.put('S');
					break;
				}
				if(tiles[j][i].isDiscovered())
				{
					discoveredMapFile.put('*');
				}
				else
				{
					discoveredMapFile.put('.');
				}
			}
			mapFile.put('\n');
			discoveredMapFile.put('\n');
		}
		mapFile.close();
		discoveredMapFile.close();
	}
}


Tile::tileType Map::getTileType(int x, int y)
{
	return tiles[x][y].getType();
}


bool Map::hasMonster(int x, int y)
{
	return tiles[x][y].hasMonster();
}

	
bool Map::hasPlayer(int x, int y)
{
	return tiles[x][y].hasPlayer();
}


void Map::setPlayerFlag(int x, int y, bool flag)
{
	tiles[x][y].setPlayerFlag(flag);
}


void Map::resetMonster(int x, int y)
{
	tiles[x][y].setMonster(-1);
}


void Map::setMonster(int x, int y, int id)
{
	tiles[x][y].setMonster(id);
}


int Map::getMonster(int x, int y)
{
	return tiles[x][y].getMonster();
}


/* isBlocked() and visit() are defined for FOV calculation. 
   See permissive-fov-1.0/permissive-fov-cpp.h for details */

bool Map::isBlocked(short x, short y)
{
	return tiles[x][y].isBlocked();
}


void Map::visit(short x, short y)
{
	tiles[x][y].visit();
	if(tiles[x][y].hasMonster())
	{
		context.monsters[tiles[x][y].getMonster()]->setPlayerVisibility(true);
	}
}


bool Map::isVisible(int x, int y)
{
	return tiles[x][y].isVisible();
}


bool Map::hasItem(int x, int y)
{
	return tiles[x][y].hasItem();
}


int Map::getItem(int x, int y)
{
	return tiles[x][y].getItem();
}


void Map::setItem(int x, int y, int id)
{
	tiles[x][y].setItem(id);
	context.items[id]->setPosition(x, y);
}


void Map::removeItem(int x, int y)
{
	tiles[x][y].setItem(-1);
}


void Map::setType(int x, int y, Tile::tileType type)
{
	tiles[x][y].setType(type);
}