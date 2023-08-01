#include "Game.h"
#include <iostream>

Game::Game() {

	window = new sf::RenderWindow(sf::VideoMode(windowWidth, windowHeight), "Testing");
	// just a bit of fun
	window->setFramerateLimit(120);
	window->setVerticalSyncEnabled(false);

	if (!playerTexture.loadFromFile("Player.png"))
	{
		throw std::exception("Failed to load Player image.");
		return;
	}

	if (!wallTexture.loadFromFile("Wall.png"))
	{
		throw std::exception("Failed to load Wall image.");
		return;
	}

	if (!finishTexture.loadFromFile("Finish.png"))
	{
		throw std::exception("Failed to load Finish image.");
		return;
	}

	playerTexture.setSmooth(true);
	wallTexture.setSmooth(true);
	finishTexture.setSmooth(true);

	// seed random now, rather than in the world initalisation.
	srand(time(nullptr));

	InitWorld(true);
}

Game::~Game()
{
	delete window;
}

void Game::InitWorld(const bool random, const std::string& worldFile)
{
	std::cout << "Loading " << (random ? "a random" : worldFile) << " world..." << std::endl;
	
	// World can be initalised more than once
	// we need to kill everything that exists if there is anything that exists.
	if (tiles.size() != 0) {
		tiles.empty();
	}

	if (gridLines.size() != 0) {
		gridLines.empty();
	}

	playerPosX = 0;
	startPlayerPosX = 0;
	playerPosY = 0;
	startPlayerPosY = 0;

	// This is where the do the loading of the world if random wasn't selected.
	// for now, only allow random.

	// Pre-determine the size of each grid cell.
	const int gridCellSizeX = windowWidth / gridAmount;
	const int gridCellSizeY = windowHeight / gridAmount;

	// if grid is 4x4, we make 3 lines on the x axis.
	for (int i = 0; i <= gridAmount - 1; i++) {
		gridLines.emplace_back(std::make_unique<Line>(
			sf::Vector2f(gridCellSizeX * (i+1), 0), 
			sf::Vector2f(gridCellSizeX * (i + 1), windowHeight)));
	}

	// Then we do it again for the y axis.
	for (int i = 0; i <= gridAmount - 1; i++) {
		gridLines.emplace_back(std::make_unique<Line>(
			sf::Vector2f(0, gridCellSizeY * (i + 1)),
			sf::Vector2f(windowWidth, gridCellSizeY * (i + 1))));
	}

	// Set the size of the tiles vector
	tiles.resize(gridAmount);
	// Now we need to set the size of each column vector.
	for (auto& column : tiles)
		column.resize(gridAmount);

	// Now we make tiles for all the grid slots.
	// the first loop is the rows.
	for (int i = 0; i <= gridAmount - 1; i++) {
		// this loop (second) is the columns
		// this means i=row, j=column.
		for (int j = 0; j <= gridAmount - 1; j++) {

			tiles[i][j] = std::make_unique<Tile>(
				sf::Vector2f((gridCellSizeX * (j+1)) - (gridCellSizeX / 2)
					,(gridCellSizeY * (i + 1)) - (gridCellSizeY / 2)));
		}
	}
	
	// random number between 0 and max row (or max col)
	unsigned int ranX = rand() % gridAmount;
	unsigned int ranY = rand() % gridAmount;
	
	// Set the random tile to the player texture
	tiles[ranY][ranX]->SetTileSprite(playerTexture);
	tiles[ranY][ranX]->typeOfTile = TileType::Player;
	// set current position and start position to the location picked.
	playerPosX = ranX;
	startPlayerPosX = ranX;
	playerPosY = ranY;
	startPlayerPosY = ranY;

	// Now get new random values again to set the finish texture
	unsigned int ranX_ = 9999;
	unsigned int ranY_ = 9999;

	// if ranX_ or ranY_ is 0 or the both values are the same as the player's location
	// then reset the new locations
	while (ranX_ == 9999 || ranY_ == 9999 || (ranX_ == ranX && ranY_ == ranY)) {
		ranX_ = rand() % gridAmount;
		ranY_ = rand() % gridAmount;
	}

	// Set finish point to the random location picked.
	tiles[ranY_][ranX_]->SetTileSprite(finishTexture);
	tiles[ranY_][ranX_]->typeOfTile = TileType::Finish;
}

void Game::render() {
	window->clear(sf::Color::White);

	// Draw all grid lines.
	for (auto const& line : gridLines) {

		const sf::Vertex tempLine[] =
		{
			sf::Vertex(line->GetStartLocation(), sf::Color::Black),
			sf::Vertex(line->GetEndLocation(), sf::Color::Black)
		};

		// verts, vertexcount, type
		window->draw(tempLine, 2, sf::Lines);
	}

	// Go through all tiles and draw their information.
	for (auto& column : tiles) {
		for (auto& tile : column) {

			// If a tile hasn't been created correctly but placed in the vector
			// this will pick it up.
			if (!tile) {
				throw std::runtime_error("Found an invalid tile.");
			}

			// sprites can't have an invalid texture if it has been set
			// so we use a TileType to determine what the tile is.
			// If it's blank, we don't draw the sprite.
			if (tile->typeOfTile != TileType::Blank)
				window->draw(tile->sprite);

			// num text comes OVER everything else.
			window->draw(tile->numText);

		}
	}

	window->display();
}

void Game::update()
{
	pollEvents();
}

void Game::pollEvents()
{
	while (window->pollEvent(ev)) {
		switch (ev.type) {
			case sf::Event::Closed:
				window->close();
				break;
			case sf::Event::KeyPressed:
				if (ev.key.code == sf::Keyboard::Escape) {
					window->close();
				}
				else if (ev.key.code == sf::Keyboard::F5) {
					// save world
				}
				else if (ev.key.code == sf::Keyboard::W ||
					ev.key.code == sf::Keyboard::S) {
					// Process movement on Y axis.
					// If W was pressed, move up (1), otherwise move down (-1).
					ProcessMovement(sf::Vector2f(
						.0f,
						ev.key.code == sf::Keyboard::W ? 1.f : -1.f));
				}
				else if(ev.key.code == sf::Keyboard::A || 
					ev.key.code == sf::Keyboard::D) {
					// Process movement on X axis.
					// If A was pressed, move left (-1), otherwise move right (1).
					ProcessMovement(sf::Vector2f(
						ev.key.code == sf::Keyboard::A ? -1.f : 1.f,
						0.f));
				}

				break;
		}
	}
}

void Game::ResetGame()
{
	std::cout << "Resetting game." << "\n";
	InitWorld(true);
}

void Game::ProcessMovement(const sf::Vector2f& inputValue)
{
	tiles[playerPosY][playerPosX]->typeOfTile = TileType::Blank;

	// Only move on x/y if the value isn't 0.
	if (inputValue.x != 0)
		// If inputx is below 0, then - playerPosX by the value, otherwise add it.
		playerPosX = inputValue.x < 0 ? playerPosX - std::abs(inputValue.x) : playerPosX + inputValue.x;

	if (inputValue.y != 0)
		// same as before but add the negative value (so w can be 1) and minus the positive value (so s can be -1).
		// otherwise, w has to be -1 and s has to be 1 (which is numerically right but I like 1 = up, -1 = down.
		// Think like Unreal's GetForwardVector and GetRightVector, this is essentially what i'm doing.
		playerPosY = inputValue.y < 0 ? playerPosY + std::abs(inputValue.y) : playerPosY - inputValue.y;

	// clamp the values so they don't go out of the array index.
	// First we check if it's higher than the grid max, if it is then set to max y, otherwise keep the same.
	playerPosX = playerPosX > gridAmount - 1 ? gridAmount - 1 : playerPosX;
	// Then we want to see if the player is trying to go below 0, if so keep them at 0, otherwise keep the same.
	playerPosX = playerPosX < 0 ? 0 : playerPosX;

	// Repeat the process for the Y axis.
	playerPosY = playerPosY > gridAmount - 1 ? gridAmount - 1 : playerPosY;
	playerPosY = playerPosY < 0 ? 0 : playerPosY;

	// do checks here for if that tile is the finish.

	if (tiles[playerPosY][playerPosX]->typeOfTile == TileType::Finish) {
		ResetGame();
	}

	tiles[playerPosY][playerPosX]->typeOfTile = TileType::Player;
	tiles[playerPosY][playerPosX]->SetTileSprite(playerTexture);
}
