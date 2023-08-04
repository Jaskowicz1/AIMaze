#pragma once

#include "NumCpp.hpp"

class Game;

class AI {

public:

	AI(const int sizeOfBoard, const bool loadQTable);

	void TrainAI(Game* gameRef);
	
	void TrainingFunc(Game* gameRef);
	
	void DoCompletedPath(Game* gameRef);

	int EpsilonGreedyPolicy(int rowNum);

	int GreedyPolicy(int rowNum) {
		return nc::argmax(QTable.row(rowNum))[0];
	}

	nc::NdArray<int> QTable;

	int currentTrainingEpisode;

	bool isTrainable = true;

protected:

	float RandomNum();

	int RandomAction();

	float epsilon = .0f;

	const int possibleActions = 4;

	const int n_training_episodes = 1000;
	const float learning_rate = 0.7f;

	const int n_eval_episodes = 100;

	const int max_steps = 99;
	const float gamma = 0.95f;

	const float max_epsilon = 1.0f;
	const float min_epsilon = 0.05f;
	const float decay_rate = 0.0005f;
};
