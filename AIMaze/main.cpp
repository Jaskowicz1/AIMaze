#include "SFML/Graphics.hpp"

#include "main.h"

using namespace Main;

int main()
{
	game = std::make_unique<Game>();

	game->Initiate();

	while (game->IsRunning()) {
		game->update();
		game->render();
	}
	
	return 0;
}