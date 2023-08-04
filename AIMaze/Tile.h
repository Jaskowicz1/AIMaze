#pragma once
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "TileType.h"

class Tile
{

public:

	Tile(const sf::Vector2f& centre, const int ID);

	sf::Vector2f tileCentre;

	// The type of tile.
	TileType typeOfTile = TileType::Blank;

	sf::Sprite sprite;

	void SetTileSprite(const sf::Texture& texture);

	int GetTileID()
	{
		return tileID;
	}
protected:

	int tileID;

};