#pragma once
#include "Creature.h"
#include "Game.h"

struct GameContext;
class Game;


class Player : public Creature
{
public:
    Player(GameContext& context);
    Player(std::string name, GameContext& context);
    void save(boost::property_tree::ptree& parent);
    void load(boost::property_tree::ptree& tree);
    void loadSprite();
    Creature::MoveResult moveLeft();
    Creature::MoveResult moveRight();
    Creature::MoveResult moveUp();
    Creature::MoveResult moveDown();
    Creature::MoveResult move(int x, int y);
    bool processDeath();
    void setProperties();
    void setPosition(int x, int y);
    int getATK();
    int getDEF();
    int getSpeed();
    int getMaxHP();
    int getLevel();
    int getXP();
    int getXPToNextLvl();
    void pickUpItem(int id);
    int dropItem(int i);
    bool hasInventorySpace();
    bool hasItem(int id);
    void levelUp();
    void addXP(int xp);
    void restoreHP();

private:
    sf::Texture texture;
    int STR;
    int DEX;
    int level;
    int maxHP;
    int XP;
    int XPToNextLvl;
    int speed;
    Game* game;
    std::vector<int> inventory;
    static const int INVENTORY_SLOTS = 10;
    static const int XP_MULTIPLIER = 1500;
};

