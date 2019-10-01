#pragma once
#include <SFML/Graphics.hpp>
#include <sstream>
#include "Messenger.h"
#include "GameContext.h"

struct GameContext;

class Hud
{
public:
    Hud(GameContext& context);
    void draw();
    void writeMessage(std::ostringstream& msg);
    void flush();
    sf::Font font;

private:
    struct
    {
        std::string name;
        std::string level;
        std::string XP;
        std::string HP;
        std::string floor;
    } playerStatus;
    GameContext& context;
    Messenger messenger;

    void updatePlayerStatus();
};

