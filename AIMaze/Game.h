#pragma once

#include "SFML/Graphics.hpp"
#include "Line.h"
#include "Tile.h"

class Game {

public:

	Game();
	virtual ~Game();

#pragma region Non-Game Functions
	
	void render();
	void update();
	void pollEvents();

#pragma endregion

#pragma region Game Functions

	void InitWorld(const bool random, const std::string& worldFile = "");

	void ResetGame();

	void ProcessMovement(const sf::Vector2f& inputValue);

	const bool IsRunning() {
		return this->window->isOpen();
	}

#pragma endregion

#pragma region Grid Stuff

	std::vector<std::unique_ptr<Line>> gridLines;

	std::vector<std::vector<std::unique_ptr<Tile>>> tiles;

#pragma endregion

private:

	// How many rows and columns?
	const int gridAmount = 4;

	const unsigned int windowWidth = 1280;
	const unsigned int windowHeight = 720;

#pragma region SFML Stuff

	sf::Texture playerTexture;
	sf::Texture wallTexture;
	sf::Texture finishTexture;

	sf::RenderWindow* window;
	sf::Event ev;

#pragma endregion

	// Player Position
	int playerPosX, playerPosY = 0;
	int startPlayerPosX, startPlayerPosY = 0;

};