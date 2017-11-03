#include "ChatWindow.h"

// Chat.cpp
ChatWindow::ChatWindow() {
	linewidth = 20;

	size.height = 400;
	size.width = 600;
	size.top = 0;
	size.left = 0;

	if (!font.loadFromFile("Crimson-Roman.ttf"))
	{
		// error...
		//return 0;
	}
	tempText.setFont(font);
	tempText.setCharacterSize(32);
	tempText.setFillColor(sf::Color::Red);
	tempText.setStyle(sf::Text::Bold);

	renderWindow.create(sf::VideoMode(size.width*2, size.height*2), "Awesome Chat Window");
}

void ChatWindow::add_string(const std::string& s) {
	// Move all other strings one line up.
	for (auto it = chatlog.begin(); it < chatlog.end(); it++) {
		sf::Vector2f temp = it->getPosition();
		temp.y -= linewidth;
		it->setPosition(temp);
	}
	// Add the new string to the bottom of the chat.
	tempText.setString(s);
	int xPos = size.left;
	int yPos = size.top + size.height*2 - linewidth;
	tempText.setPosition(sf::Vector2f(xPos, yPos));
	chatlog.push_back(tempText);

	if (chatlog.size() > MAX_LINES) {
		chatlog.pop_front();
	}
}

void ChatWindow::render() {
	sf::Event evnt;
	while (renderWindow.pollEvent(evnt)) 
	{
		switch (evnt.type) 
		{
		case sf::Event::Closed:
			renderWindow.close();
			break;
		}
	}
	renderWindow.clear();
	for (auto it : chatlog) {
		renderWindow.draw(it);
	}
	renderWindow.display();
}

