#pragma once
#include <SFML/Graphics.hpp>
#include <deque>
#include <sstream>


class Messenger
{
public:
    Messenger(sf::RenderWindow& window, sf::Font& font);
    void writeMessage(std::ostringstream& msg);
    void display();
    void flush();

private:
    static const int MAX_LINES = 10;
    static const int LINE_HEIGHT = 15;
    sf::RenderWindow& window;
    sf::Font& font;
    std::deque<std::string> messages;
    sf::Text texts[MAX_LINES];
    sf::RectangleShape background;
    void scroll();
};

