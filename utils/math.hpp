#ifndef MATH_H
#define MATH_H

#include <SFML/System/Vector2.hpp>
#include <cmath>

struct Math {
	static constexpr float PI = 3.1415936f;
	static sf::Vector2f dot(sf::Vector2f v1, sf::Vector2f v2) { return {v1.x * v2.x, v1.y * v2.y}; }
	static float dot_mag(sf::Vector2f v1, sf::Vector2f v2) { return v1.x * v2.x + v1.y * v2.y; }
	static float magnitude(sf::Vector2f v) { return sqrt(v.x * v.x + v.y * v.y); }
};

#endif
