#include "Tile.h"
#include <iostream>

Tile::Tile(const sf::Vector2f& centre, const int id)
{
	tileCentre = centre;
	tileID = id;

	// Set sprite position
	sprite.setPosition(centre);
	// Make sprite bigger.
	sprite.scale(sf::Vector2f(1.5f, 1.5f));
}

void Tile::SetTileSprite(const sf::Texture& texture)
{
	// Update sprite texture
	sprite.setTexture(texture);

	// Set origin of texture so it's centred (only works once texture has been set).
	sprite.setOrigin(sf::Vector2f(32.f, 32.f));
}
