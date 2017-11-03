#pragma once

#include <iostream>
#include <SFML\System.hpp>
#include <SFML\Network.hpp>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <string>
#include <thread>
#include <mutex>
#include <deque>
#include <thread>

class ChatWindow {
private:
	const static int MAX_LINES = 25;
	int linewidth;
	sf::IntRect size;
	sf::Font font;
	sf::Text tempText;
	std::deque<sf::Text> chatlog;

	sf::RenderWindow renderWindow;
public:
	ChatWindow();
	void add_string(const std::string& s);
	void render();
};

