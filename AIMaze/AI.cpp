#include "AI.h"

#include "Game.h"

AI::AI(const int sizeOfBoard, const bool loadQTable)
{
	QTable = nc::zeros<int>(sizeOfBoard * sizeOfBoard, possibleActions);
	
	if(loadQTable)
	{
		std::ifstream MazeTableFile("CompletedMazeTable.txt");
		std::string qtableLine;

		int row = 0;
		int col = 0;

		std::cout << "Loading Q-Table..." << "\n";
		
		while (std::getline(MazeTableFile, qtableLine))
		{
			std::cout << std::stoi(qtableLine) << "\n";
			
			QTable(row, col) = std::stoi(qtableLine);

			if(col % (sizeOfBoard - 1) == 0)
			{
				row++;
				col = 0;
			}

			col++;
		}

		//MazeTableFile.close();

		//boost::detail::Sleep(1000);
		
		
		std::cout << "Loaded Q-Table!" << "\n";
	}

	std::cout << "AI is alive and has " << (loadQTable ? "loaded the saved " : "generated a ") << " Q-Table." << "\n";

	isTrainable = !loadQTable;

	//std::cout << QTable << "\n";

	if(loadQTable)
		std::cout << "Since a Q-Table has been loaded, the AI will NOT be able to train." << "\n";
}

void AI::TrainAI(Game* gameRef)
{
	if(!isTrainable)
	{
		std::cout << "The AI is not trainable since it has loaded a Q-Table." << "\n";
		return;
	}
	
	TrainingFunc(gameRef);
}

void AI::TrainingFunc(Game* gameRef)
{
	std::cout << "Training has begun." << "\n";

	std::ifstream qtableSaved("CompletedMazeTable.txt");

	if (qtableSaved.good())
	{
		char input;
		
		std::cout << "A saved QTable already exists and will be overwritten. Are you sure you want to continue? (Y/n)" << "\n";

		std::cin >> input;

		if(input == 'N' || input == 'n')
		{
			std::cout << "Understood. Stopping AI." << "\n";
			return;
		} else if (input != 'Y' && input != 'y')
		{
			std::cout << "Did not understand input. Assuming no." << "\n";
			return;
		}

		qtableSaved.clear();
	}
	
	for (int i = 0; i <= n_training_episodes - 1; i++) {

		// Call update so we can poll events (other wise the window shows as "not responding").
		gameRef->update();
		// Render as well because rendering in on same thread as training.
		// this does slow down training but it's fine.
		gameRef->render();

		std::cout << "Training episode: " << i << "/" << n_training_episodes << "\n";

		// Calculate epsilon.
		epsilon = min_epsilon + (max_epsilon - min_epsilon) * nc::exp(-decay_rate * i);

		// Keep references to player position and tile as we don't want it to reset every loop.
		int playerRow = gameRef->playerPosY;
		int playerCol = gameRef->playerPosX;
		int currentTileID = gameRef->tiles[playerRow][playerCol]->GetTileID();

		for (int j = 0; j <= max_steps - 1; j++) {

			// Calculate what action we should do.
			int action = EpsilonGreedyPolicy(gameRef->tiles[playerRow][playerCol]->GetTileID());

			// Get information from the move we just did.
			// Would be better to use a class to hand over the information for this rather than a std::vector but
			// it works for now (just maybe not the best way to do it).
			std::vector<float> info = gameRef->ProcessAiMovement(action);

			float reward = 0;
			bool finished = false;

			// Make sure that info isn't empty.
			if(!info.empty())
			{
				reward = info[0];
				finished = info[1] == 1;
			}

			// Get a reference to the new positions (these are allowed to reset every loop, hence why they are here
			// and also const.
			const int newPlayerRow = gameRef->playerPosY;
			const int newPlayerCol = gameRef->playerPosX;
			const int newTileID = gameRef->tiles[newPlayerRow][newPlayerCol]->GetTileID();

			// Update the QTable with the new value from the current action.
			QTable(currentTileID, action) = QTable(currentTileID, action) + learning_rate * (reward + (gamma * nc::max(QTable.row(newTileID))[0]) - QTable(currentTileID, action));

			// Tell the game that the AI has finished movement and calculations.
			gameRef->FinishedAIUpdate();

			playerRow = newPlayerRow;
			playerCol = newPlayerCol;
			currentTileID = gameRef->tiles[playerRow][playerCol]->GetTileID();

			// Do update and render AGAIN because we've done movement.
			gameRef->update();
			gameRef->render();

			// If the AI finished the game, move on to the next 
			if (finished)
				break;

			//boost::detail::Sleep(500);
		}
	}

	std::cout << "Dumping QTable to 'CompletedMazeTable.txt'!" << "\n";
	std::cout << QTable << "\n";
	QTable.tofile("CompletedMazeTable.txt", '\n');
}

void AI::DoCompletedPath(Game* gameRef)
{
	bool finished = false;

	std::cout << "Proceeding to follow the complete path." << "\n";
	
	while(!finished)
	{
		const int playerRow = gameRef->playerPosY;
		const int playerCol = gameRef->playerPosX;
		const int tileID = gameRef->tiles[playerRow][playerCol]->GetTileID();
		
		int action = GreedyPolicy(tileID);

		// Get information from the move we just did.
		// Would be better to use a class to hand over the information for this rather than a std::vector but
		// it works for now (just maybe not the best way to do it).
		std::vector<float> info = gameRef->ProcessAiMovement(action);

		// Make sure that info isn't empty.
		if(!info.empty())
		{
			finished = info[1] == 1;
		}

		// Tell the game that the AI has finished movement and calculations.
		gameRef->FinishedAIUpdate();

		// Do update and render AGAIN because we've done movement.
		gameRef->update();
		gameRef->render();

		// So the user can actually see the AI move properly.
		boost::detail::Sleep(500);
	}

	std::cout << "The AI has reached the end!" << "\n";
}

int AI::EpsilonGreedyPolicy(const int rowNum)
{
	int ranNum = RandomNum();

	if (ranNum > epsilon)
		return nc::argmax(QTable.row(rowNum))[0];

	return RandomAction();
}

float AI::RandomNum()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(0, 100000);

	return static_cast<float>(distrib(gen)) / 100000;
}

int AI::RandomAction()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(0, 3);

	return distrib(gen);
}
