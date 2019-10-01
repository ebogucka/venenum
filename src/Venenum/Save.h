#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <unordered_map>
#include "Game.h"
#include "Creature.h"
#include "Player.h"
#include "GameContext.h"

class Creature;

class Save
{
public:
    Save(GameContext& context);
    void store();
    void restore();

private:
    GameContext& context;
};

