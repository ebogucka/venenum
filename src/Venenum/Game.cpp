#include "Game.h"
#include "Utils.h"


Game::Game() : state(Game::UNINITIALIZED), isPlaying(false)
{
	context.window = new sf::RenderWindow(sf::VideoMode(800, 600), "Venenum", sf::Style::Titlebar);
	context.hud = new Hud(context);
	context.map = new Map(context);
	context.player = new Player(context);
	context.floor = 0;
	context.monsters.clear();
	context.items.clear();
	FOVmask = new permissive::maskT("data/fov.dat");
}


void Game::initialize()
{
	if(state != Game::UNINITIALIZED)
	{
		return;
	}

	context.window->setFramerateLimit(60);
	guiView = context.window->getDefaultView();
	mainView = context.window->getDefaultView();

	Creature::loadData(context);
	Item::loadData(context);
	save = new Save(context);
	context.player->loadSprite();
	Tile::loadTextures(*context.hud);
	
	state = Game::SHOWING_MENU;
}


void Game::loop()
{
	while(context.window->isOpen())
    {
		switch(state)
		{
		case Game::SHOWING_MENU:
			showMenu();
			break;

		case Game::PLAYER_TURN:
			draw();
			processPlayerTurn();
			break;

		case Game::AI_TURN:
			processAITurn();
			break;

		case Game::GAME_OVER:
			showDeathScreen();
			break;

		case Game::GAME_WON:
			congratulate();
			break;

		case Game::EXITING:
			context.window->close();
			break;
		}
	}
}


void Game::processPlayerTurn()
{
	context.player->restoreHP();
	sf::Event event;
	int actions = 0;
	while(context.window->pollEvent(event));

	while(actions < context.player->getSpeed())
	{
		context.window->waitEvent(event);
		if(event.type == sf::Event::Closed)
		{
			state = Game::EXITING;
			return;
		}
		if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		{
			state = Game::SHOWING_MENU;
			return;
		}

		Creature::MoveResult result = Creature::NONE;

		if(event.type == sf::Event::KeyPressed)
		{
			if(event.key.code == sf::Keyboard::A)
			{
				result = context.player->moveLeft();
			}
			else if(event.key.code == sf::Keyboard::D)
			{
				result = context.player->moveRight();
			}
			else if(event.key.code == sf::Keyboard::S)
			{
				result = context.player->moveDown();
			}
			else if(event.key.code == sf::Keyboard::W)
			{
				result = context.player->moveUp();
			}

			if(result == Creature::OK || result == Creature::NEXT_FLOOR)
			{
				actions++;
				updateView();
				draw();
				if(context.player->getAnimationFlag())
				{
					animate(context.player);
				}
				if(result == Creature::NEXT_FLOOR)
				{
					context.floor++;
					generateLevel();
				}
			}
			else if(result == Creature::PLAYER_WON)
			{
				state = Game::GAME_WON;
				return;
			}
		}
	}
	state = Game::AI_TURN;
}


void Game::processAITurn()
{
	Creature::MoveResult result = Creature::NONE;
	for(std::unordered_map<int, Creature *>::iterator it = context.monsters.begin(); it != context.monsters.end(); ++it)
	{
		for(int i = 0; i < it->second->getSpeed(); i++)
		{
			result = it->second->act();
			if(it->second->canSeePlayer())
			{
				draw();
				if(it->second->getAnimationFlag())
				{
					animate(it->second);
				}
				else
				{
					sf::sleep(sf::milliseconds(300));
				}
			}
			if(result == Creature::PLAYER_KILLED)
			{
				isPlaying = false;
				state = Game::GAME_OVER;
				return;
			}
		}
	}
	state = PLAYER_TURN;
}


void Game::draw()
{
	context.window->clear();
	context.window->setView(mainView);
	context.map->draw();
	context.player->draw();
	context.window->setView(guiView);
	context.hud->draw();
	context.window->display();
}


void Game::updateView()
{
	context.map->resetVisibility();
	mainView.setCenter(context.player->getPosition().x * Tile::SIZE, context.player->getPosition().y * Tile::SIZE);
	permissive::fov(context.player->getPosition().x, context.player->getPosition().y, *FOVmask, *context.map);
}


void Game::showMenu()
{
	MainMenu mainMenu(100, 100, 40, *context.window);

	if(Utils::fileExists("data/save.dat") || isPlaying)
	{
		mainMenu.addItem("Resume Game", MenuItem::RESUME_GAME);
	}
	mainMenu.addItem("New Game", MenuItem::NEW_GAME);
	if(isPlaying)
	{
		mainMenu.addItem("Save & Quit", MenuItem::SAVE_AND_QUIT);
	}
	else
	{
		mainMenu.addItem("Quit", MenuItem::QUIT);
	}

	MenuItem::MenuResult result = mainMenu.getMenuResponse();

	switch(result)
	{
	case MenuItem::NEW_GAME:
		context.hud->flush();
		context.floor = 1;
		context.player->setProperties();
		generateLevel();
		break;

	case MenuItem::RESUME_GAME:
		if(!isPlaying)
		{
			save->restore();
			updateView();
			isPlaying = true;
		}
		state = Game::PLAYER_TURN;
		break;

	case MenuItem::SAVE_AND_QUIT:
		save->store();
		state = Game::EXITING;
		break;

	case MenuItem::QUIT:
		state = Game::EXITING;
		break;
	}
}


void Game::generateLevel()
{
	reset();
	if(context.floor == Game::LAST_FLOOR)
	{
		context.items.insert(std::make_pair<int, Item*>(context.items.size(), new Item(Item::ANTIDOTUM, context, context.items.size())));
	}
	context.map->generate();
	std::ostringstream msg;
	msg << context.player->getName() << " reaches " << context.floor << " floor.";
	context.hud->writeMessage(msg);
	int nMonsters = rand() % 31 + 20;
	while(context.monsters.size() < nMonsters)
	{
		Creature::CreatureType monsterType = static_cast<Creature::CreatureType>(rand() % 5 + (context.floor-1) * 2);
		int x, y;
		do
		{
			x = rand() % Map::MAP_WIDTH;
			y = rand() % Map::MAP_HEIGHT;
		} while(!context.map->isTilePassable(x, y));
		context.monsters.insert(std::make_pair<int, Creature*>(context.monsters.size(), new Creature(monsterType, x, y, context, context.monsters.size())));
	}

	updateView();
	draw();
	isPlaying = true;
	state = Game::PLAYER_TURN;
}


void Game::reset()
{
	if(!context.monsters.empty())
	{
		for(std::unordered_map<int, Creature *>::iterator it = context.monsters.begin(); it != context.monsters.end(); ++it)
		{
			delete it->second;
		}
		context.monsters.clear();
	}
	if(!context.monsters.empty())
	{
		for(std::unordered_map<int, Creature *>::iterator it = context.monsters.begin(); it != context.monsters.end(); ++it)
		{
			if(!context.player->hasItem(it->second->getID()))
			{
				delete it->second;
			}
			context.items.erase(it->first);
		}
	}
}


void Game::showDeathScreen()
{
	sf::sleep(sf::seconds(3));
	context.window->clear(sf::Color::Black);
	sf::Text text("YOU DIED");
	text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2, text.getLocalBounds().top + text.getLocalBounds().height / 2);
	text.setPosition(context.window->getSize().x / 2, context.window->getSize().y / 2);
	text.setColor(sf::Color::Red);
	context.window->draw(text);
	context.window->display();

	sf::Event event;
	while(context.window->pollEvent(event));
	while(context.window->waitEvent(event))
	{
		if(event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed)
		{
			state = Game::SHOWING_MENU;
			return;
		}
	}
}


void Game::congratulate()
{
	sf::sleep(sf::seconds(3));
	context.window->clear(sf::Color::Black);
	sf::Text text("Congratulations!");
	text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2, text.getLocalBounds().top + text.getLocalBounds().height / 2);
	text.setPosition(context.window->getSize().x / 2, context.window->getSize().y / 2 - 100);
	text.setColor(sf::Color::Green);
	context.window->draw(text);
	text.setString("You have found the antidotum you were looking for for!");
	text.setOrigin(text.getLocalBounds().left + text.getLocalBounds().width / 2, text.getLocalBounds().top + text.getLocalBounds().height / 2);
	text.move(0, 150);
	context.window->draw(text);
	context.window->display();

	sf::Event event;
	while(context.window->pollEvent(event));
	while(context.window->waitEvent(event))
	{
		if(event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed)
		{
			state = Game::SHOWING_MENU;
			return;
		}
	}
}

void Game::animate(Creature* creature)
{
	sf::sleep(sf::milliseconds(150));
	creature->moveSprite(0, 10);
	creature->setAnimationFlag(false);
	draw();
	sf::sleep(sf::milliseconds(150));
}