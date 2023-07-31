#pragma once

#include "SFML/Graphics.hpp"
#include "Line.h"
#include "Tile.h"

class Game {

public:

	Game();
	virtual ~Game();

	const bool IsRunning() {
		return this->window->isOpen();
	}

	void InitWorld(bool random);

	void render();
	void update();
	void pollEvents();

	std::vector<std::unique_ptr<Line>> gridLines;

	std::vector<std::vector<std::unique_ptr<Tile>>> tiles;


private:

	// How many rows and columns?
	const int gridAmount = 4;

	const unsigned int windowWidth = 1280;
	const unsigned int windowHeight = 720;

	sf::Texture playerTexture;
	sf::Texture wallTexture;
	sf::Texture finishTexture;

	sf::RenderWindow* window;
	sf::Event ev;


};