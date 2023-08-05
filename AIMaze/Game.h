#pragma once

#include "SFML/Graphics.hpp"
#include "Line.h"
#include "Tile.h"
#include "AI.h"

class Game {

public:

	Game();
	virtual ~Game();

	inline void FinishedAIUpdate()
	{
		UpdatePlayerTile(true);
	}

#pragma region Non-Game Functions
	
	void render();
	void update();
	void pollEvents();

#pragma endregion

#pragma region Game Functions

	void InitWorld(const bool random, const std::string& worldFile = "");

	void Initiate();

	void ResetGame(const bool newWorld);

	void ProcessPlayerMovement(const sf::Vector2f& inputValue);

	std::vector<float> ProcessAiMovement(const int action);

	inline const bool IsRunning() {
		return this->window->isOpen();
	}

#pragma endregion

#pragma region Grid Stuff

	std::vector<std::unique_ptr<Line>> gridLines;

	std::vector<std::vector<std::unique_ptr<Tile>>> tiles;

	// Player Positions
	int playerPosX = 9999;
	int playerPosY = 9999;
	
	// Finish Positions
	int finishPosX = 9999;
	int finishPosY = 9999;

	int lastDistance = 0;

#pragma endregion

	std::unique_ptr<AI> aiRef;
	//std::thread aiThread;

private:

	// How many rows and columns?
	const int gridAmount = 5;

	const unsigned int windowWidth = 1280;
	const unsigned int windowHeight = 720;

#pragma region SFML Stuff

	sf::Texture playerTexture;
	sf::Texture wallTexture;
	sf::Texture finishTexture;

	sf::RenderWindow* window;
	sf::Event ev;

#pragma endregion

	void ClampPlayerPosition();

	void UpdatePlayerTile(bool ai);

	void UpdateAllTileText();

	// Player Start Positions
	int startPlayerPosX, startPlayerPosY = 0;

	const int amountOfWalls = 3;

};