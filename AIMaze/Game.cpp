#include "Game.h"
#include <iostream>

Game::Game()
{
	window = new sf::RenderWindow(sf::VideoMode(windowWidth, windowHeight), "AI Maze project.");
	// just a bit of fun
	window->setFramerateLimit(120);
	window->setVerticalSyncEnabled(false);

	if (!playerTexture.loadFromFile("Player.png"))
	{
		std::cout << "Failed to load Player image.";
		return;
	}

	if (!wallTexture.loadFromFile("Wall.png"))
	{
		std::cout << "Failed to load Wall image.";
		return;
	}

	if (!finishTexture.loadFromFile("Finish.png"))
	{
		std::cout << "Failed to load Finish image.";
		return;
	}

	playerTexture.setSmooth(true);
	wallTexture.setSmooth(true);
	finishTexture.setSmooth(true);
}

Game::~Game()
{
	delete window;
}

void Game::render()
{
	window->clear(sf::Color::White);

	// Draw all grid lines.
	for (const auto& line : gridLines) {

		const sf::Vertex tempLine[] =
		{
			sf::Vertex(line->GetStartLocation(), sf::Color::Black),
			sf::Vertex(line->GetEndLocation(), sf::Color::Black)
		};

		// verts, vertexcount, type
		window->draw(tempLine, 2, sf::Lines);
	}

	// Go through all tiles and draw their information.
	for (const auto& column : tiles) {
		for (const auto& tile : column) {

			// If a tile hasn't been created correctly but placed in the vector
			// this will pick it up.
			if (!tile)
				continue;

			// sprites can't have an invalid texture if it has been set
			// so we use a TileType to determine what the tile is.
			// If it's blank, we don't draw the sprite.
			if (tile->typeOfTile != TileType::Blank)
				window->draw(tile->sprite);
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
			// Had to disable this because it seems to keep getting called for no reason?
			//case sf::Event::Closed:
				//window->close();
				//break;
			case sf::Event::KeyPressed:
				if (ev.key.code == sf::Keyboard::Escape) {
					window->close();
				}
				else if (ev.key.code == sf::Keyboard::W ||
					ev.key.code == sf::Keyboard::S) {
					// Process movement on Y axis.
					// If W was pressed, move up (1), otherwise move down (-1).
					ProcessPlayerMovement(sf::Vector2f(
						.0f,
						ev.key.code == sf::Keyboard::W ? 1.f : -1.f));
				}
				else if(ev.key.code == sf::Keyboard::A || 
					ev.key.code == sf::Keyboard::D) {
					// Process movement on X axis.
					// If A was pressed, move left (-1), otherwise move right (1).
					ProcessPlayerMovement(sf::Vector2f(
						ev.key.code == sf::Keyboard::A ? -1.f : 1.f,
						0.f));
				}

				break;
		}
	}
}

void Game::InitWorld(const bool random, const std::string& worldFile)
{
	std::cout << "Loading " << (random ? "a random" : worldFile) << " world..." << std::endl;
	
	// World can be initialised more than once
	// we need to kill everything that exists if there is anything that exists.
	if (!tiles.empty()) {
		tiles.clear();
	}

	if (!gridLines.empty()) {
		gridLines.clear();
	}

	playerPosX = 0;
	startPlayerPosX = 0;
	playerPosY = 0;
	startPlayerPosY = 0;

	// Allow a seed, this means we don't have to "save" the world
	// as the same seed will give the same world.
	std::default_random_engine generator( 556555 );
	//std::random_device rd;
	std::mt19937 gen(generator());
	std::uniform_int_distribution<> distrib(0, gridAmount - 1);

	// This is where the do the loading of the world if random wasn't selected.
	// for now, only allow random.

	// Pre-determine the size of each grid cell.
	const float gridCellSizeX = windowWidth / gridAmount;
	const float gridCellSizeY = windowHeight / gridAmount;

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

	int tileID = 0;

	// Now we make tiles for all the grid slots.
	// the first loop is the rows.
	for (int i = 0; i <= gridAmount - 1; i++) {
		// this loop (second) is the columns
		// this means i=row, j=column.
		for (int j = 0; j <= gridAmount - 1; j++) {

			tiles[i][j] = std::make_unique<Tile>(
				sf::Vector2f((gridCellSizeX * (j+1)) - (gridCellSizeX / 2)
					,(gridCellSizeY * (i + 1)) - (gridCellSizeY / 2)), tileID);
			tileID++;
		}
	}

	// random number between 0 and max row (or max col)
	playerPosX = distrib(gen);
	playerPosY = distrib(gen);
	
	// Set the random tile to the player texture
	tiles[playerPosY][playerPosX]->SetTileSprite(playerTexture);
	tiles[playerPosY][playerPosX]->typeOfTile = TileType::Player;
	
	// set current position and start position to the location picked.
	startPlayerPosX = playerPosX;
	startPlayerPosY = playerPosY;

	std::cout << "Created the player and set starting position..." << "\n";

	// Now get new random values again to set the finish texture

	finishPosX = 9999;
	finishPosY = 9999;

	// if ranX_ or ranY_ is 0 or the both values are the same as the player's location
	// then reset the new locations
	while (finishPosX == 9999 || finishPosY == 9999 || (finishPosX == playerPosX && finishPosY == playerPosY)) {
		finishPosX = distrib(gen);
		finishPosY = distrib(gen);
	}
	
	// Set finish point to the random location picked.
	tiles[finishPosY][finishPosX]->SetTileSprite(finishTexture);
	tiles[finishPosY][finishPosX]->typeOfTile = TileType::Finish;

	std::cout << "Created the finish and set finish position..." << "\n";

	// Set the distance between the player and the finish for the AI.
	lastDistance = std::abs(playerPosX - finishPosX) + std::abs(playerPosY - finishPosY);

	for(int i=0; i<=amountOfWalls-1; i++)
	{
		int ranWallX = 9999;
		int ranWallY = 9999;

		while (ranWallX == 9999 || ranWallY == 9999 || (ranWallX == playerPosX && ranWallY == playerPosY)
			|| (ranWallX == finishPosX && ranWallY == finishPosY)) {
			ranWallX = distrib(gen);
			ranWallY = distrib(gen);
		}

		// Set finish point to the random location picked.
		tiles[ranWallY][ranWallX]->SetTileSprite(wallTexture);
		tiles[ranWallY][ranWallX]->typeOfTile = TileType::Wall;
	}

	std::cout << "Created a random world!" << "\n";
}

void Game::Initiate()
{
	InitWorld(true);

	render();

	char input;

	bool useSavedTable = false;

	std::cout << "Do you wish to train the AI? No (N/n) will result in the AI playing its known Q-Table. (Y/n)" << "\n";

	std::cin >> input;

	if(input == 'N' || input == 'n')
	{
		std::cout << "Understood. Will not train the AI." << "\n";
		useSavedTable = true;
		
	} else if (input != 'Y' && input != 'y')
	{
		std::cout << "Did not understand input. Assuming no." << "\n";
		useSavedTable = true;
	}

	aiRef = std::make_unique<AI>(gridAmount, useSavedTable);
	
	if(!useSavedTable)
	{
		std::cout << "Attempting to train AI..." << "\n";
		aiRef->TrainAI(this);
	}
	else
		aiRef->DoCompletedPath(this);
}

void Game::ResetGame(const bool newWorld)
{
	if (newWorld) {
		InitWorld(true);
		return;
	}

	playerPosX = startPlayerPosX;
	playerPosY = startPlayerPosY;
}

void Game::ProcessPlayerMovement(const sf::Vector2f& inputValue)
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

	// Clamp the player's location because we don't want the player trying to go out-of-bounds.
	ClampPlayerPosition();

	// Update the player tile.
	UpdatePlayerTile(false);
}

std::vector<float> Game::ProcessAiMovement(const int action)
{
	// This vector holds the reward and if we're finished (first value = reward, second value = finished).
	// finished is either 0 or 1. 1 being finished.
	// This definitely is not the best way to do it but we're doing it this way.
	std::vector<float> values;

	tiles[playerPosY][playerPosX]->typeOfTile = TileType::Blank;

	int lastPosX = playerPosX;
	int lastPosY = playerPosY;

	// ACTION VALUE KEY:
	// 0 = Up
	// 1 = Down
	// 2 = Left
	// 3 = Right.

	// if action is 0 or 1.
	if (action == 0 || action == 1) {
		// if 0, go up (decrease y by 1). Otherwise, go down (increase y by 1).
		playerPosY = action == 0 ? playerPosY - 1 : playerPosY + 1;
	}
	else {
		// If 2, go left (decrease x by 1). Otherwise, go right (increase x by 1).
		playerPosX = action == 2 ? playerPosX - 1 : playerPosX + 1;
	}

	ClampPlayerPosition();

	float reward = 0.f;

	int distance = std::abs(playerPosX - finishPosX) + std::abs(playerPosY - finishPosY);

	// Calculate the distance to the finish with the new player position.
	// if the last distance was smaller than the the new distance, we've gone the wrong way.
	if(distance > lastDistance)
	{
		// We now give a negative reward.
		// We also increase how negative the reward was, depending on how far away we've gone.
		// this will mean that tiles further away are given worse numbers than tiles closer.
		reward -= (50.f + distance);
	} else if(distance < lastDistance) // if the last distance is bigger, we moved closer to the finish.
	{
		// We now give a positive reward
		// we increase the reward by how close we are to the finish. The closer we are, the bigger the reward.
		// This should tell the AI that it's getting closer and closer rather than just saying "right step, wrong step".
		reward += (10.f + distance);
	} else if(distance == lastDistance) // if we didn't get any closer (stayed in the same space) then that's bad.
	{
		// We give a negative reward for this, more severe than anything else.
		// This is because the AI should be encouraged to still try random ways, but should not be encouraged to go
		// into the maze's actual walls.
		reward -= 9999.f;
	}

	lastDistance = distance;

	if(lastPosX == playerPosX && lastPosY == playerPosY)
	{
		// we stayed the same place, make sure the AI KNOWS that was a bad move.
		reward -= 9999.f;
		values.push_back(reward);
		values.push_back(0.f);

		// Because we know the outcome, don't bother with anything below, we've already punished the AI enough.
		return values;
	}

	switch (tiles[playerPosY][playerPosX]->typeOfTile)
	{
		case TileType::Wall:
			// We do not want to reward a negative value for hitting a wall. Whilst its bad, it'll make the tile before
			// show as really bad, this means we should just leave it as zero.
			values.push_back(reward); // Reward
			values.push_back(1.f); // completed (0 = false, 1 = true);
			break;
		case TileType::Finish:
			reward += 9999.f;
			values.push_back(reward);
			values.push_back(1.f);
			break;
		default: // We'll assume blank here.
			// No reward for a blank tile, but the AI will get an award for moving closer (if they did).
			values.push_back(reward);
			values.push_back(0.f);
			break;
	}

	return values;
}

void Game::ClampPlayerPosition()
{
	// clamp the values so they don't go out of the array index.
	// First we check if it's higher than the grid max, if it is then set to max y, otherwise keep the same.
	playerPosX = playerPosX > gridAmount - 1 ? gridAmount - 1 : playerPosX;
	// Then we want to see if the player is trying to go below 0, if so keep them at 0, otherwise keep the same.
	playerPosX = playerPosX < 0 ? 0 : playerPosX;

	// Repeat the process for the Y axis.
	playerPosY = playerPosY > gridAmount - 1 ? gridAmount - 1 : playerPosY;
	playerPosY = playerPosY < 0 ? 0 : playerPosY;
}

void Game::UpdatePlayerTile(bool ai)
{
	if (tiles[playerPosY][playerPosX]->typeOfTile == TileType::Finish || tiles[playerPosY][playerPosX]->typeOfTile == TileType::Wall) {
		//ResetGame(!ai);
	}

	tiles[playerPosY][playerPosX]->typeOfTile = TileType::Player;
	tiles[playerPosY][playerPosX]->SetTileSprite(playerTexture);
}

void Game::UpdateAllTileText()
{
	//for(auto& col : tiles)
	//{
	//	for(auto& tile : col)
	//	{
	//		tile->SetTileNum(aiRef->QTable[playerPosX * playerPosY - 1]);
	//	}
	//}
}
