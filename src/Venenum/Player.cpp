#include <cstdio>
#include <boost/foreach.hpp>
#include "Player.h"


Player::Player(GameContext& context) : Creature(context)
{
    name = "Hero";
    setProperties();
}


Player::Player(std::string name, GameContext& context) : Creature(context)
{
    this->name = name;
    setProperties();
}


void Player::save(boost::property_tree::ptree& tree)
{
    tree.put("game.player.name", name);
    tree.put("game.player.position.x", position.x);
    tree.put("game.player.position.y", position.y);
    tree.put("game.player.STR", STR);
    tree.put("game.player.DEX", DEX);
    tree.put("game.player.HP", HP);
    tree.put("game.player.speed", speed);
    tree.put("game.player.level", level);
    tree.put("game.player.XP", XP);
    tree.put("game.player.inventory", "");
    for(int i = 0; i < inventory.size(); i++)
    {
        tree.put("game.player.inventory.item", inventory[i]);
    }
}


void Player::load(boost::property_tree::ptree& tree)
{
    name = tree.get<std::string>("game.player.name");
    setPosition(tree.get<int>("game.player.position.x"), tree.get<int>("game.player.position.y"));
    STR = tree.get<int>("game.player.STR");
    DEX = tree.get<int>("game.player.DEX");
    HP = tree.get<int>("game.player.HP");
    speed = tree.get<int>("game.player.speed");
    level = tree.get<int>("game.player.level");
    XP = tree.get<int>("game.player.XP");
    XPToNextLvl = level * Player::XP_MULTIPLIER;
    BOOST_FOREACH(boost::property_tree::ptree::value_type &value, tree.get_child("game.player.inventory"))
    {
        inventory.push_back(value.second.get<int>(""));  
    }
}


void Player::loadSprite()
{
    std::string image = "../images/player.png";
    if(!texture.loadFromFile(image))
    {
        std::ostringstream msg;
        msg << "Failed to load " << image << "!";
        context.hud->writeMessage(msg);
    }
    sprite.setTexture(texture);
}


Creature::MoveResult Player::moveLeft()
{
    return move(position.x - 1, position.y);
}


Creature::MoveResult Player::moveRight()
{
    return move(position.x + 1, position.y);
}


Creature::MoveResult Player::moveUp()
{
    return move(position.x, position.y - 1);
}


Creature::MoveResult Player::moveDown()
{
    return move(position.x, position.y + 1);
}


Creature::MoveResult Player::move(int x, int y)
{
    if(context.map->getTileType(x, y) == Tile::STAIRS)
    {
        context.map->setPlayerFlag(position.x, position.y, false);
        setPosition(x, y);
        return Creature::NEXT_FLOOR;
    }
    if(context.map->isTilePassable(x, y))
    {
        context.map->setPlayerFlag(position.x, position.y, false);
        setPosition(x, y);
        if(context.map->hasItem(x, y))
        {
            if(context.items[context.map->getItem(x, y)]->getType() == Item::ANTIDOTUM)
            {
                return Creature::PLAYER_WON;
            }
            if(hasInventorySpace())
            {
                pickUpItem(context.map->getItem(x, y));
                context.map->removeItem(x, y);
            }
        }
        return Creature::OK;
    }
    else if(context.map->hasMonster(x, y))
    {
        attack(context.monsters[context.map->getMonster(x, y)]);
        return Creature::OK;
    }
    else
    {
        return Creature::FAILED;
    }
}


bool Player::processDeath()
{
    remove("../data/save.dat");
    remove("../data/map.dat");
    remove("../data/map2.dat");
    return true;
}


void Player::setProperties()
{
    maxHP = 50;
    HP = maxHP;
    STR = 10;
    DEX = 10;
    speed = 3;
    level = 1;
    XP = 0;
    XPToNextLvl = Player::XP_MULTIPLIER;
}


void Player::setPosition(int x, int y)
{
    context.map->setPlayerFlag(x, y, true);
    position.x = x;
    position.y = y;
    sprite.setPosition(x * Tile::SIZE, y * Tile::SIZE);
}


int Player::getATK()
{
    return STR;
}


int Player::getDEF()
{
    return DEX;
}


int Player::getSpeed()
{
    return speed;
}


void Player::pickUpItem(int id)
{
    inventory.push_back(id);
}


int Player::dropItem(int i)
{
    int id = inventory[i];
    inventory.erase(inventory.begin() + i);
    return id;
}


bool Player::hasInventorySpace()
{
    return inventory.size() < Player::INVENTORY_SLOTS;
}


bool Player::hasItem(int id)
{
    return std::find(inventory.begin(), inventory.end(), id) != inventory.end();
}


void Player::levelUp()
{
    STR = STR + 2;
    DEX = DEX + 2;
    maxHP = maxHP * 1.50;
    HP = maxHP;
    level++;
    XPToNextLvl = level * Player::XP_MULTIPLIER;
    std::ostringstream msg;
    msg << name << " is now Level " << level << "!";
    context.hud->writeMessage(msg);
}


void Player::addXP(int xp)
{
    XP += xp;
    if(XP >= XPToNextLvl)
    {
        XP -= XPToNextLvl;
        levelUp();
    }
}


int Player::getMaxHP()
{
    return maxHP;
}


int Player::getLevel()
{
    return level;
}


int Player::getXP()
{
    return XP;
}


int Player::getXPToNextLvl()
{
    return XPToNextLvl;
}


void Player::restoreHP()
{
    if(HP < maxHP)
    {
        HP += level;
        if(HP >= maxHP)
        {
            HP = maxHP;
        }
    }
}
