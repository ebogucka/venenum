#include "Creature.h"
#include "Player.h"


Creature::Creature(GameContext& context) : context(context), animation(false)
{
}


Creature::Creature(Creature::CreatureType type, GameContext& context, int id) : type(type), context(context), id(id), animation(false)
{
	setProperties();
}


Creature::Creature(Creature::CreatureType type, int x, int y, GameContext& context, int id) : type(type), context(context), id(id), animation(false)
{
	setProperties();
	setPosition(x, y);
}


void Creature::saveState(boost::property_tree::ptree& tree)
{
	boost::property_tree::ptree child;
	child.put("id", id);
	child.put("type", type);
	child.put("position.x", position.x);
	child.put("position.y", position.y);
	child.put("HP", HP);
	tree.add_child("game.monsters.monster", child);
}


void Creature::loadState(boost::property_tree::ptree& tree, GameContext& context)
{	
	int id = tree.get<int>("id");
	enum Creature::CreatureType loadedType = static_cast<Creature::CreatureType>(tree.get<int>("type"));
	Creature* loadedMonster = new Creature(loadedType, context, id);
	loadedMonster->setPosition(tree.get<int>("position.x"), tree.get<int>("position.y"));
	loadedMonster->HP = tree.get<int>("HP");
	context.monsters.insert(std::make_pair(id, loadedMonster));
}


void Creature::loadData(GameContext& context)
{
	using namespace boost::property_tree;
	ptree file;
	read_xml("../data/game.dat", file);

	int i = 0;
	BOOST_FOREACH(ptree::value_type &value, file.get_child("monsters"))
	{
		Creature::data.push_back(new Creature::CreatureData());
		Creature::data[i]->name = value.second.get<std::string>("name");
		Creature::data[i]->HP = value.second.get<int>("HP");
		Creature::data[i]->ATK = value.second.get<int>("ATK");
		Creature::data[i]->DEF = value.second.get<int>("DEF");
		Creature::data[i]->speed = value.second.get<int>("speed");
		Creature::data[i]->XPGained = value.second.get<int>("XPGained");
		Creature::data[i]->texture = new sf::Texture();
		Creature::data[i]->texture->loadFromFile("../images/monsters/" + value.second.get<std::string>("image"));
		i++;
	}
}


Creature::MoveResult Creature::act()
{
	if(canSeePlayer())
	{
		return chasePlayer();
	}
	else
	{
		return moveRandomly();
	}
}


Creature::MoveResult Creature::chasePlayer()
{
	sf::Vector2u playerPosition = context.player->getPosition();
	sf::Vector2i direction = sf::Vector2i(playerPosition - position);
	
	int number;

	if(direction.x == 0)
	{
		number = 1;
	}
	else if(direction.y == 0)
	{
		number = 0;
	}
	else
	{
		number = rand() % 2;
	}

	Creature::MoveResult result = Creature::NONE;
	switch(number)
	{
	case 0:
		if(direction.x > 0)
		{	
			result = moveRight();
		}
		else if(direction.x < 0)
		{
			result = moveLeft();
		}
		break;
	case 1:
		if(direction.y > 0)
		{
			result = moveDown();
		}
		else if(direction.y < 0)
		{
			result = moveUp();
		}
		break;
	}

	if(result == Creature::OK || result == Creature::PLAYER_KILLED)
	{
		return result;
	}

	return moveRandomly();
}


Creature::MoveResult Creature::moveRandomly()
{
	bool finished = false;
	Creature::MoveResult result = Creature::NONE;
	int attempts = 0;
	while(!finished && attempts < 10)
	{
		int number = rand() % 4;

		switch(number)
		{
		case 0:
			result = moveUp();
			break;
		case 1:
			result = moveRight();
			break;
		case 2:
			result = moveDown();
			break;
		case 3:
			result = moveLeft();
			break;
		}

		if(result == Creature::OK || result == Creature::PLAYER_KILLED)
		{
			return result;
		}
		attempts++;
	}
	return Creature::FAILED;
}


bool Creature::attack(Creature* defender)
{
	sprite.move(0, -10);
	animation = true;

	int damage = Combat::calculateDamage(getATK(), defender->getDEF());
	std::ostringstream msg;
	if(damage > 0)
	{
		defender->decreaseHP(damage);
		msg << name << " deals " << damage << " damage to " << defender->getName() << ".";
	}
	else
	{
			msg << name << " misses!";
	}
	context.hud->writeMessage(msg);
	if(defender->isDead())
	{
		bool result = defender->processDeath(); // 0 - monster died, 1 - player died
		if(result == false)
		{
			delete defender;
		}
		return result;
	}
	return false;
}


void Creature::draw()
{
	context.window->draw(sprite);
}
	

Creature::MoveResult Creature::moveLeft()
{
	return move(position.x - 1, position.y);
}


Creature::MoveResult Creature::moveRight()
{
	return move(position.x + 1, position.y);
}


Creature::MoveResult Creature::moveUp()
{
	return move(position.x, position.y - 1);
}


Creature::MoveResult Creature::moveDown()
{
	return move(position.x, position.y + 1);
}


Creature::MoveResult Creature::move(int x, int y)
{
	if(context.map->isTilePassable(x, y))
	{
		context.map->resetMonster(position.x, position.y);
		setPosition(x, y);
		return Creature::OK;
	}
	else if(context.map->hasPlayer(x, y))
	{
		if(attack((Creature *)context.player))
		{
			return Creature::PLAYER_KILLED;
		}
		else
		{
			return Creature::OK;
		}
	}
	return Creature::FAILED;
}


bool Creature::processDeath()
{
	std::ostringstream msg;
	msg << name << " dies!";
	context.hud->writeMessage(msg);
	context.map->resetMonster(position.x, position.y);
	context.monsters.erase(id);
	context.player->addXP(Creature::data[type]->XPGained);
	return false;
}


sf::Vector2u Creature::getPosition()
{
	return position;
}


void Creature::setPosition(int x, int y)
{
	context.map->setMonster(x, y, id);
	position.x = x;
	position.y = y;
	sprite.setPosition(x * Tile::SIZE, y * Tile::SIZE);
}


void Creature::setProperties()
{
	name = Creature::data[type]->name;
	HP = Creature::data[type]->HP;
	sprite.setTexture(*Creature::data[type]->texture);
	playerVisible = false;
}


std::string Creature::getName()
{
	return name;
}


int Creature::getID(void)
{
	return id;
}


void Creature::decreaseHP(int damage)
{
	HP -= damage;
	if(HP < 0)
	{
		HP = 0;
	}
}

int Creature::getATK()
{
	return Creature::data[type]->ATK;
}


int Creature::getDEF()
{
	return Creature::data[type]->DEF;
}


int Creature::getHP()
{
	return HP;
}


int Creature::getSpeed()
{
	return Creature::data[type]->speed;
}


bool Creature::isDead()
{
	return HP == 0;
}


bool Creature::canSeePlayer()
{
	return playerVisible;
}


void Creature::setPlayerVisibility(bool flag)
{
	playerVisible = flag;
}


void Creature::moveSprite(float x, float y)
{
	sprite.move(x, y);
}


bool Creature::getAnimationFlag()
{
	return animation;
}


void Creature::setAnimationFlag(bool flag)
{
	animation = flag;
}

std::vector<Creature::CreatureData*> Creature::data;