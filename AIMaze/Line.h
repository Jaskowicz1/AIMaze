#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Vertex.hpp>

class Line {

public:

	Line(const sf::Vector2f& start, const sf::Vector2f& end);

	sf::Vector2f GetStartLocation() {
		return startLoc;
	}

	sf::Vector2f GetEndLocation() {
		return endLoc;
	}

protected:

	sf::Vector2f startLoc;
	sf::Vector2f endLoc;
};