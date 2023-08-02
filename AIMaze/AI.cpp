#include "AI.h"

#include <stdlib.h> 
#include "Game.h"

AI::AI(const int sizeOfBoard)
{
	QTable = nc::zeros<int>(sizeOfBoard, possibleActions);

	std::cout << "AI is alive and has created a Q-Table." << "\n";
}

void AI::TrainAI(Game* gameRef)
{
	//std::thread thread_object(&AI::TrainAI,);
	TrainingFunc(gameRef);
}

void AI::TrainingFunc(Game* gameRef)
{
	std::cout << "Training has begun." << "\n";
	
	for (int i = 0; i <= n_training_episodes - 1; i++) {

		std::cout << "Episode: " << i << " / " << n_training_episodes << "\n";
		
		epsilon = min_epsilon + (max_epsilon - min_epsilon) * nc::exp(-decay_rate * i);

		std::cout << "epsilon: " << epsilon << " / " << max_epsilon << "\n";

		int playerRow = gameRef->playerPosY;

		for (int j = 0; j <= max_steps - 1; j++) {

			//std::cout << "Step: " << j << " / " << max_steps << "\n";

			int action = EpsilonGreedyPolicy(playerRow);

			//std::cout << "Action = " << action << "\n";
			
			std::vector<float> info = gameRef->ProcessAiMovement(action);

			//std::cout << "Processed movement! " << "\n";

			float reward = 0;
			bool finished = false;

			if(info.empty())
			{
				std::cout << "Info is empty!" << "\n";
				std::cout << "Continuing under assumption that reward is 0 and not finished!" << "\n";
			} else
			{
				reward = info[0];
				finished = info[1] == 1;
			}

			//std::cout << "Got values! " << "\n";

			int newPlayerRow = gameRef->playerPosY;

			//std::cout << "I'm about to update QTable! " << "\n";

			QTable.row(playerRow)[action] = QTable.row(playerRow)[action] + learning_rate
				* (reward + gamma * nc::max(QTable.row(newPlayerRow))[0]
					- QTable.row(playerRow)[action]);

			//std::cout << "Updated QTable! " << "\n";

			if (finished)
				break;

			playerRow = newPlayerRow;
			//playerPosition = newPlayerPosition;

			//std::cout << "Set playerRow and playerPosition." << "\n";
		}
	}
}

int AI::EpsilonGreedyPolicy(int rowNum)
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
