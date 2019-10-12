#include "Messenger.h"

Messenger::Messenger(sf::RenderWindow& window, sf::Font& font) : window(window), font(font)
{
    for(int i = 0; i < MAX_LINES; i++)
    {
        texts[i].setFont(font);
        texts[i].setCharacterSize(26);
        texts[i].setStyle(sf::Text::Regular);
        texts[i].setFillColor(sf::Color::White);
        texts[i].setPosition(20, window.getSize().y - ((float) messages.size() * LINE_HEIGHT) - 100);
        messages.push_back("");
    }
}


void Messenger::writeMessage(std::ostringstream& msg)
{
    messages.push_back(msg.str());
    msg.str("");
    scroll();
}


void Messenger::display()
{
    scroll();
    window.draw(background);
    for(unsigned int i = 0; i < messages.size(); i++)
    {
        window.draw(texts[i]);
    }
}

void Messenger::scroll()
{
    int removedLines = 0;
    while(messages.size() > MAX_LINES)
    {
        messages.pop_front();
    }
    for(unsigned int i = 0; i < messages.size(); i++)
    {
        texts[i].setString(messages[i]);
    }
}


void Messenger::flush()
{
    for(int i = 0; i < MAX_LINES; i++)
    {
        messages.push_back("");
    }
}
