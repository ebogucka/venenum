#include "Menu.h"


Menu::Menu(int x, int y, int itemHeight, sf::RenderWindow& window)
    : position(x, y), itemHeight(itemHeight), activeItem(0), window(window)
{
    font.loadFromFile("../fonts/sansation.ttf");
}


Menu::~Menu()
{
    for(unsigned int i = 0; i < items.size(); i++)
    {
        delete items[i];
    }
}


MenuItem::MenuResult Menu::handleClick(int x, int y)
{
    for(unsigned int i = 0; i < items.size(); i++)
    {
        if(items[i]->contains(x, y))
        {
            return items[i]->getAction();
        }
    }

    return MenuItem::NOTHING;
}


void Menu::handleHover(int x, int y)
{
    for(unsigned int i = 0; i < items.size(); i++)
    {
        if(items[i]->contains(x, y))
        {
            activeItem = i;
            update();
        }
    }
}


MenuItem::MenuResult Menu::getMenuResponse()
{
    update();
    sf::Event menuEvent;

    while(window.isOpen())
    {
        while(window.pollEvent(menuEvent))
        {
            if(menuEvent.type == sf::Event::MouseButtonPressed)
            {
                return handleClick(menuEvent.mouseButton.x, menuEvent.mouseButton.y);
            }
            if(menuEvent.type == sf::Event::MouseMoved)
            {
                handleHover(menuEvent.mouseMove.x, menuEvent.mouseMove.y);
            }
            if(menuEvent.type == sf::Event::KeyPressed)
            {
                if(menuEvent.key.code == sf::Keyboard::Down || menuEvent.key.code == sf::Keyboard::S)
                {
                    activeItem++;
                    if(activeItem >= items.size())
                    {
                        activeItem = 0;
                    }
                    update();
                }
                if(menuEvent.key.code == sf::Keyboard::Up || menuEvent.key.code == sf::Keyboard::W)
                {
                    activeItem--;
                    if(activeItem < 0)
                    {
                        activeItem = items.size() - 1;
                    }
                    update();
                }
                if(menuEvent.key.code == sf::Keyboard::Return)
                {
                    return items[activeItem]->getAction();
                }
            }
        }
    }
}


void Menu::addItem(std::string label, MenuItem::MenuResult action)
{
    MenuItem* item = new MenuItem(label, action, position.x, position.y + items.size() * itemHeight);
    item->setFont(font);
    item->setCharacterSize(60);
    items.push_back(item);
}


void Menu::update()
{
    for(unsigned int i = 0; i < items.size(); i++)
    {
        if(i == activeItem)
        {
            items[i]->setColor(sf::Color(221, 196, 136));
        }
        else
        {
            items[i]->setColor(sf::Color::White);
        }
    }
    show();
}
