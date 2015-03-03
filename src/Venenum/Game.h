#pragma once
#include <SFML/Graphics.hpp>
#include "permissive-fov-1.0/permissive-fov-cpp.h"
#include <sstream>
#include "GameContext.h"
#include "Save.h"
#include "MainMenu.h"
#include "Item.h"


class Utils;
class Menu;
class Save;
class Creature;

class Game
{
public:
	static enum GameState
	{
		UNINITIALIZED,
		SHOWING_MENU, 
		CHARACTER_CREATION, 
		GENERATING_WORLD,
        PLAYER_TURN,
		AI_TURN, 
		SHOWING_INVENTORY,
		SHOWING_SPELLS,
		SHOWING_MAP, 
		SHOWING_STATS, 
		GAME_OVER,
		GAME_WON,
		EXITING 
	};

	static const int LAST_FLOOR = 5;

	Game();
	void initialize();
	void loop();

private:
	void processPlayerTurn();
	void processAITurn();
	void draw();
	void updateView();
	void showMenu();
	void generateLevel();
	void reset();
	void showDeathScreen();
	void congratulate();
	void animate(Creature* creature);

    Game::GameState state;
	sf::View mainView;
	sf::View guiView;
	struct GameContext context;
	Save* save;
	permissive::maskT* FOVmask;
	bool isPlaying;
	int floor;
};