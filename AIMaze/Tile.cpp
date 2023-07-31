#include "Tile.h"
#include <iostream>

Tile::Tile(const sf::Vector2f& centre)
{
	tileCentre = centre;

	if (!font.loadFromFile("Arial.ttf"))
	{
		throw std::exception("Failed to load Arial font.");
		return;
	}

	numText.setFont(font);

	numText.setCharacterSize(18);
	numText.setPosition(centre);
	numText.setFillColor(sf::Color::Black);

	sprite.setPosition(centre);
	sprite.scale(sf::Vector2f(1.5f, 1.5f));
	
	SetTileNum(.0f);
}

void Tile::SetTileNum(const float value)
{
	tileNum = value;

	numText.setString(std::to_string(tileNum));

	// Correct text origin as num may not be same size.
	numText.setOrigin(numText.getGlobalBounds().getSize() / 2.f
		+ numText.getLocalBounds().getPosition());
}

void Tile::SetTileSprite(const sf::Texture& texture)
{
	sprite.setTexture(texture);

	sprite.setOrigin(sf::Vector2f(32.f, 32.f));
}
