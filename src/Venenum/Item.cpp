#include "Item.h"


Item::Item(Item::ItemType type, GameContext& context, int id) : type(type), context(context), id(id)
{
	setProperties();
}


void Item::draw()
{
	context.window->draw(sprite);
}


void Item::saveState(boost::property_tree::ptree& tree)
{
	boost::property_tree::ptree child;
	child.put("id", id);
	child.put("type", type);
	child.put("position.x", position.x);
	child.put("position.y", position.y);
	tree.add_child("game.items.item", child);
}


void Item::loadState(boost::property_tree::ptree& tree, GameContext& context)
{
	int id = tree.get<int>("id");
	enum Item::ItemType loadedType = static_cast<Item::ItemType>(tree.get<int>("type"));
	Item* loadedItem = new Item(loadedType, context, id);
	loadedItem->setPosition(tree.get<int>("position.x"), tree.get<int>("position.y"));
	context.items.insert(std::make_pair(id, loadedItem));
}


void Item::loadData(GameContext& context)
{
	using namespace boost::property_tree;
	ptree file;
	read_xml("../data/game.dat", file);

	int i = 0;
	BOOST_FOREACH(ptree::value_type &value, file.get_child("items"))
	{
		Item::data.push_back(new Item::ItemData());
		Item::data[i]->name = value.second.get<std::string>("name");
		Item::data[i]->texture = new sf::Texture();
		Item::data[i]->texture->loadFromFile("../images/items/" + value.second.get<std::string>("image"));
		i++;
	}
}


void Item::setProperties()
{
	name = Item::data[type]->name;
	sprite.setTexture(*Item::data[type]->texture);
}


void Item::setPosition(int x, int y)
{
	position.x = x;
	position.y = y;
	sprite.setPosition(x * Tile::SIZE, y * Tile::SIZE);
}


Item::ItemType Item::getType()
{
	return type;
}


std::vector<Item::ItemData*> Item::data;
