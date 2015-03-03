#include "Hud.h"
#include "Player.h"


Hud::Hud(GameContext& context) : context(context), messenger(*context.window, font)
{
	font.loadFromFile("fonts/sansation.ttf");
}


void Hud::draw()
{
	messenger.display();
	updatePlayerStatus();
	sf::Text playerInfo;
	playerInfo.setFont(font);
	playerInfo.setCharacterSize(15);
	playerInfo.setPosition(0, 0);
	playerInfo.setString(playerStatus.name);
	context.window->draw(playerInfo);
	playerInfo.move(0, 20);
	playerInfo.setString(playerStatus.level);
	context.window->draw(playerInfo);
	playerInfo.move(0, 20);
	playerInfo.setString(playerStatus.XP);
	context.window->draw(playerInfo);
	playerInfo.move(0, 20);
	playerInfo.setString(playerStatus.HP);
	context.window->draw(playerInfo);
	playerInfo.move(0, 20);
	playerInfo.setString(playerStatus.floor);
	context.window->draw(playerInfo);
}


void Hud::writeMessage(std::ostringstream& msg)
{
	messenger.writeMessage(msg);
}


void Hud::flush()
{
	messenger.flush();
}


void Hud::updatePlayerStatus()
{
	std::ostringstream stream;
	playerStatus.name = context.player->getName();
	stream << "LVL " << context.player->getLevel();
	playerStatus.level = stream.str();
	stream.clear();
	stream.str("");
	stream << "XP " << context.player->getXP() << "/" << context.player->getXPToNextLvl();
	playerStatus.XP = stream.str();
	stream.clear();
	stream.str("");
	stream << "HP " << context.player->getHP() << "/" << context.player->getMaxHP();
	playerStatus.HP = stream.str();
	stream.clear();
	stream.str("");
	stream << "Floor " << context.floor;
	playerStatus.floor = stream.str();
	stream.clear();
	stream.str("");
}
