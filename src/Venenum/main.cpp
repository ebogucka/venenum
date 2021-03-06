/*  main.cpp
    
    This is entry point for the game.

    The game uses the following libraries:
    * SFML (http://www.sfml-dev.org/),
    * Boost v1.53.0 (http://www.boost.org/),
    * Permissive-fov v1.0 (http://www.xmission.com/~tyrecius/permissive-fov-1.0.zip).

    Copyright 2013 by Ela Bogucka.
*/


#include "Game.h"


int main()
{    
    srand((unsigned int) time(NULL));
    Game game;
    game.initialize();
    game.loop();

    return 0;
}
