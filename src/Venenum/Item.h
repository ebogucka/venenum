#pragma once
#include <SFML/Graphics.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp> 
#include <unordered_map>
#include "GameContext.h"


struct GameContext;

class Item
{
public:
	enum ItemType
	{
		ANTIDOTUM,
		SWORD
	};

	struct ItemData
	{
		sf::Texture* texture;
		std::string name;
	};

	Item(Item::ItemType type, GameContext& context, int id);
	void draw();
	void pickUp() {}
	void drop() {}
	void saveState(boost::property_tree::ptree& parent);
	static void loadState(boost::property_tree::ptree& tree, GameContext& context);
	static void loadData(GameContext& context);
	virtual void setProperties();
	void setPosition(int x, int y);
	Item::ItemType getType();

protected:
	int id;
	sf::Sprite sprite;
	Item::ItemType type;
	std::string name;
	GameContext& context;
	static std::vector<Item::ItemData*> data;
	sf::Vector2i position;
};

