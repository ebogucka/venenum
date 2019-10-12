#pragma once
#include <SFML/Graphics.hpp>


class Combat
{
public:
    static int calculateDamage(int atk, int def)
    {
        int damage = (int) (rand() / (double) RAND_MAX * atk);
        int defense = (int) (rand() / (double) RAND_MAX * def);
        if (damage > defense)
        {
            return damage - defense;
        }
        else
        {
            return 0;
        }
    }
};
