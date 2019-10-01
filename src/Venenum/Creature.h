#pragma once
#include <SFML/Graphics.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <unordered_map>
#include "GameContext.h"
#include "Combat.h"


struct GameContext;

class Creature
{
public:
	enum CreatureType
	{
		MUSHROOM,
		LITTLE_MUSHROOM,
		RAT,
		BAT,
		WASP,
		SNAIL,
		SPIDER,
		SKELETON,
		GIANT_SKELETON,
		GHOUL,
		NECROMANCER,
		HARPY,
		EVIL_INTEGRAL
	};

	enum MoveResult
	{
		OK,
		FAILED,
		PLAYER_KILLED,
		PLAYER_WON,
		NEXT_FLOOR,
		NONE
	};

	struct CreatureData
	{
		sf::Texture* texture;
		std::string name;
		int HP;
		int ATK;
		int DEF;
		int speed;
		int XPGained;
	};

	Creature(GameContext& context);
	Creature(Creature::CreatureType type, GameContext& context, int id);
	Creature(Creature::CreatureType type, int x, int y, GameContext& context, int id);
	void saveState(boost::property_tree::ptree& tree);
	static void loadState(boost::property_tree::ptree& tree, GameContext& context);
	static void loadData(GameContext& context);
	Creature::MoveResult act();
	Creature::MoveResult chasePlayer();
	Creature::MoveResult moveRandomly();
	bool attack(Creature* defender);
	void draw();
	virtual Creature::MoveResult moveLeft();
	virtual Creature::MoveResult moveRight();
	virtual Creature::MoveResult moveUp();
	virtual Creature::MoveResult moveDown();
	Creature::MoveResult move(int x, int y);
	virtual bool processDeath();
	sf::Vector2u getPosition();
	virtual void setProperties();
	virtual void setPosition(int x, int y);
	std::string getName();
	int getID();
	void decreaseHP(int damage);
	virtual int getDEF();
	virtual int getATK();
	int getHP();
	virtual int getSpeed();
	bool isDead();
	bool canSeePlayer();
	void setPlayerVisibility(bool flag);
	void moveSprite(float x, float y);
	bool getAnimationFlag();
	void setAnimationFlag(bool flag);

protected:
	GameContext& context;
	sf::Vector2u position;
	bool animation;
	sf::Sprite sprite;
	std::string name;
	int HP;

private:
	Creature::CreatureType type;
	int id;
	bool playerVisible;
	static std::vector<Creature::CreatureData*> data;
};
