#include "MainMenu.h"


MainMenu::MainMenu(int x, int y, int itemHeight, sf::RenderWindow& window) : Menu(x, y, itemHeight, window) {}


void MainMenu::show()
{
    window.clear(sf::Color::Black);
    for(int i = 0; i < items.size(); i++)
    {
        items[i]->draw(window);
    }
    window.display();
}
