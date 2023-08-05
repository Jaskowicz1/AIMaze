#include "AI.h"

#include "Game.h"

AI::AI(const int sizeOfBoard, const bool loadQTable)
{
	// Initialise the Q-Table.
	QTable = std::make_unique<NdArray>(sizeOfBoard * sizeOfBoard, possibleActions);
	
	if(loadQTable)
	{
		// Get the completed q-table.
		if(!QTable->load("CompletedMazeTable.txt"))
		{
			std::cout << "No Q-Table was found! Please train the AI first." << "\n";
		}
		
		std::cout << "Loaded Q-Table!" << "\n";
	}

	std::cout << "AI is alive and has " << (loadQTable ? "loaded the saved " : "generated a ") << " Q-Table." << "\n";

	isTrainable = !loadQTable;

	// Just make sure the user knows.
	if(loadQTable)
		std::cout << "Since a Q-Table has been loaded, the AI will NOT be able to train." << "\n";
}

void AI::TrainAI(Game* gameRef)
{
	// Do not let the AI train whilst we're in non-train mode.
	if(!isTrainable)
	{
		std::cout << "The AI is not trainable since it has loaded a Q-Table." << "\n";
		return;
	}

	// Does a q-table already exist that's saved?
	// Whilst we've already asked the user if they want to train, we want to double-check.
	std::ifstream qtableSaved("CompletedMazeTable.txt");

	if (qtableSaved.good())
	{
		char input;

		// Ask for confirmation.
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

		// Close the file
		qtableSaved.close();
	}

	std::cout << "Training has begun." << "\n";
	
	for (int i = 0; i <= n_training_episodes - 1; i++) {

		// Call update so we can poll events (other wise the window shows as "not responding").
		gameRef->update();
		// Render as well because rendering in on same thread as training.
		// this does slow down training but it's fine.
		gameRef->render();

		// Tell the user which training episode we're on.
		// Should be moved to a debug option.
		std::cout << "Training episode: " << i << "/" << n_training_episodes << "\n";

		// Calculate epsilon.
		epsilon = min_epsilon + (max_epsilon - min_epsilon) * std::exp(-decay_rate * i);

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
			// This is Q(S, A) = Q(S, A) + learningrate * [reward + gamma * Q(S+1, A+1) - Q(S, A)].
			QTable->GetArray()[currentTileID][action] = QTable->GetArray()[currentTileID][action] + learning_rate * (reward + (gamma * QTable->max(newTileID)) - QTable->GetArray()[currentTileID][action]);

			// Tell the game that the AI has finished movement and calculations.
			gameRef->FinishedAIUpdate();

			// Update our vars.
			playerRow = newPlayerRow;
			playerCol = newPlayerCol;
			currentTileID = gameRef->tiles[playerRow][playerCol]->GetTileID();

			// Do update and render AGAIN because we've done movement.
			gameRef->update();
			gameRef->render();

			// If the AI finished the game, move on to the next training episode as there are no more
			// steps to take.
			if (finished)
				break;
		}
	}

	std::cout << "Training has finished!" << "\n";
	std::cout << "Dumping QTable to 'CompletedMazeTable.txt'!" << "\n";
	std::cout << QTable->ArrayToString() << "\n";
	// Dump the QTable to the file, each value will be separated with a new line.
	QTable->save("CompletedMazeTable.txt");
}

void AI::DoCompletedPath(Game* gameRef)
{
	bool finished = false;

	std::cout << "Proceeding to follow the complete path." << "\n";
	std::cout << QTable->ArrayToString() << "\n";
	
	while(!finished)
	{
		// Get the best action from the player's current location (tileID).
		int action = GreedyPolicy(gameRef->tiles[gameRef->playerPosY][gameRef->playerPosX]->GetTileID());

		// Perform the move that we got told by the Greedy Policy and retrieve information from it.
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

		// Only pause if we're not finished, otherwise we're waiting 500ms for no reason.
		if(!finished)
		{
			// So the user can actually see the AI move properly.
			boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
		}
	}

	std::cout << "The AI has reached the end!" << "\n";
}