#pragma once

#include "NdArray.h"
#include <random>
#include <iostream>
#include <fstream>
#include <boost/thread/thread.hpp> 
#include <boost/chrono.hpp>

class Game;

class AI {

public:

	AI(const int sizeOfBoard, const bool loadQTable);
	
	/**
	 * \brief The main training function for the AI. This trains the AI and saves its
	 * Q-Table to a text file once finished.
	 * \param gameRef The game to get information from.
	 */
	void TrainAI(Game* gameRef);

	/**
	 * \brief Make the AI follow the path it has created in its Q-Table.
	 * \param gameRef The game to get information from.
	 */
	void DoCompletedPath(Game* gameRef);
	
	/**
	 * \brief Get the best or a random action from a tile, depending on epsilon.
	 * \param tileID The tile's ID
	 * \return Either the best action or a random action from the tile specified. 
	 */
	inline int EpsilonGreedyPolicy(const int tileID)
	{
		// if the random number is bigger than our epsilon, we just get the best action from the new tile.
		// As GreedyPolicy already does that, we can just call that func.
		if (RandomNum() > epsilon)
			return GreedyPolicy(tileID);

		return RandomAction();
	}
	
	/**
	 * \brief Get the best action based on a tile's data.
	 * \param tileID The tile's ID.
	 * \return The action that has the best value from the tile.
	 */
	inline int GreedyPolicy(const int tileID) {

		// QTable.row(rowNum) seems to not give the result for what we're ACTUALLY asking for.
		// Is it possible .row(x) is doing -1 on our input?
		return QTable->argmax(tileID);
	}

	// The Q-Table.
	std::unique_ptr<NdArray> QTable;
	
	int currentTrainingEpisode;

	bool isTrainable = true;

protected:

	// Gets a random floating point number between 0-1.
	inline float RandomNum() {
		std::random_device rd;
		std::mt19937 gen(rd());
		// Because std doesn't have a float distribution, we do 0-100000 and just divide by 100000.
		// This gives us a number between 0-1.
		std::uniform_int_distribution<> distrib(0, 100000);
		
		return static_cast<float>(distrib(gen)) / 100000;
	}

	inline int RandomAction() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distrib(0, 3);

		return distrib(gen);
	}

#pragma region AI numbers

	float epsilon = .0f;

	const int possibleActions = 4;

	const int n_training_episodes = 5000;
	const float learning_rate = 0.7f;

	const int n_eval_episodes = 100;

	const int max_steps = 99;
	const float gamma = 0.95f;

	const float max_epsilon = 1.0f;
	const float min_epsilon = 0.05f;
	const float decay_rate = 0.0005f;
	
#pragma endregion 
};
