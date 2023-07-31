#pragma once
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

class Tile
{

public:

	Tile(const sf::Vector2f& centre);

	sf::Vector2f tileCentre;

	sf::Font font;

	sf::Sprite sprite;
	sf::Text numText;

	void SetTileNum(const float value);

	float GetTileNum() {
		return tileNum;
	}

	void SetTileSprite(const sf::Texture& texture);

protected:

	float tileNum = .0f;

};