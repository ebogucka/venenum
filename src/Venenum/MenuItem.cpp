#include "MenuItem.h"


MenuItem::MenuItem(std::string label, MenuResult action, int x, int y) : action(action)
{
    text.setString(label);
    text.setPosition((float) x, (float) y);
}


void MenuItem::draw(sf::RenderWindow& window)
{
    window.draw(text);
}


MenuItem::MenuResult MenuItem::getAction()
{
    return action;
}


void MenuItem::setColor(sf::Color color)
{
    text.setFillColor(color);
}

void MenuItem::setFont(sf::Font & font)
{
    text.setFont(font);
}

void MenuItem::setCharacterSize(int size)
{
    text.setCharacterSize(size);
}

bool MenuItem::contains(int x, int y)
{
    return text.getGlobalBounds().contains((float) x, (float) y);
}
