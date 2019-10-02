#pragma once
#include <SFML/Graphics.hpp>


class Combat
{
public:
    static int calculateDamage(int ATK, int DEF)
    {
        int damage = (rand() / RAND_MAX) * ATK;
        if(damage > (rand() / RAND_MAX) * DEF)
        {
            return damage;
        }
        else
        {
            return 0;
        }
    }
};
