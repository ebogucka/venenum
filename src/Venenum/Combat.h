#pragma once
#include <SFML/Graphics.hpp>


class Combat
{
public:
	static int calculateDamage(int ATK, int DEF)
	{
		int damage = ((double) rand() / (RAND_MAX)) * ATK;
		if(damage > ((double) rand() / (RAND_MAX)) * DEF)
		{
			return damage;
		}
		else
		{
			return 0;
		}
	}
};