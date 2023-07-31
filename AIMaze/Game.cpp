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

	InitWorld(true);
}

Game::~Game()
{
	delete window;
}

void Game::InitWorld(bool random)
{
	std::cout << "Loading " << (random ? "a random " : "") << "world..." << std::endl;

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

	// seed random
	srand(time(nullptr));
	
	// random number between 0 and max row (or max col)
	int ranX = rand() % gridAmount;
	int ranY = rand() % gridAmount;
	
	// Set the random tile to the player texture
	tiles[ranY][ranX]->SetTileSprite(playerTexture);

	// Now get new random values again to set the finish texture
	int ranX_ = 0;
	int ranY_ = 0;

	// if ranX_ or ranY_ is 0 or the both values are the same as the player's location
	// then reset the new locations
	while (ranX_ == 0 || ranY_ == 0 || (ranX_ == ranX && ranY_ == ranY)) {
		ranX_ = rand() % gridAmount;
		ranY_ = rand() % gridAmount;
	}

	tiles[ranY_][ranX_]->SetTileSprite(finishTexture);
}

void Game::render() {
	window->clear(sf::Color::White);

	for (auto const& line : gridLines) {

		const sf::Vertex tempLine[] =
		{
			sf::Vertex(line->GetStartLocation(), sf::Color::Black),
			sf::Vertex(line->GetEndLocation(), sf::Color::Black)
		};

		// verts, vertexcount, type
		window->draw(tempLine, 2, sf::Lines);
	}

	for (auto& column : tiles)
		for (auto& tile : column) {
			if (!tile) {
				throw std::exception("Found an invalid tile.");
			}
			
			window->draw(tile->sprite);

			// num text comes OVER everything else.
			window->draw(tile->numText);
			
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
				if (ev.key.code == sf::Keyboard::Escape)
					window->close();
					break;
		}
	}
}
