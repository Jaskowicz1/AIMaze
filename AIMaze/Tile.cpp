#include "Tile.h"
#include <iostream>

Tile::Tile(const sf::Vector2f& centre, const int ID)
{
	tileCentre = centre;
	tileID = ID;

	sprite.setPosition(centre);
	sprite.scale(sf::Vector2f(1.5f, 1.5f));
}

void Tile::SetTileSprite(const sf::Texture& texture)
{
	sprite.setTexture(texture);

	sprite.setOrigin(sf::Vector2f(32.f, 32.f));
}
