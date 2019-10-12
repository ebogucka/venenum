#include "Hud.h"
#include "Player.h"


Hud::Hud(GameContext& context) : context(context), messenger(*context.window, font)
{
    font.loadFromFile("../fonts/sansation.ttf");
}


void Hud::draw()
{
    messenger.display();
    updatePlayerStatus();
    sf::Text playerInfo;
    playerInfo.setFont(font);
    playerInfo.setCharacterSize(32);
    playerInfo.setPosition(20, 20);

    std::vector<std::string> texts = {
        playerStatus.name,
        playerStatus.level,
        playerStatus.XP,
        playerStatus.HP,
        playerStatus.floor
    };
    
    for each(std::string text in texts) 
    {
        playerInfo.move(0, 40);
        playerInfo.setString(text);
        context.window->draw(playerInfo);
    }
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
