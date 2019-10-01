#include "MenuItem.h"


MenuItem::MenuItem(std::string label, MenuResult action, int x, int y) : action(action)
{
    text.setString(label);
    text.setPosition(x, y);
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
    text.setColor(color);
}


bool MenuItem::contains(int x, int y)
{
    return text.getGlobalBounds().contains(x, y);
}
