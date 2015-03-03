#include "Messenger.h"

Messenger::Messenger(sf::RenderWindow& window, sf::Font& font) : window(window), font(font)
{
	for(int i = 0; i < MAX_LINES; i++)
	{
		texts[i].setFont(font);
		texts[i].setCharacterSize(12);
		texts[i].setStyle(sf::Text::Regular);
		texts[i].setColor(sf::Color::White);
		texts[i].setPosition(0.0, 450 + (messages.size() * LINE_HEIGHT));
		messages.push_back("");
	}
	background.setSize(sf::Vector2f(300, 150));
	background.setFillColor(sf::Color(0, 0, 0, 150));
	background.setPosition(0, 450);
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
	for(int i = 0; i < messages.size(); i++)
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
	for(int i = 0; i < messages.size(); i++)
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