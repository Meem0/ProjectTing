#ifndef HELPFUL_H
#define HELPFUL_H

#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>

class Helpful
{
public:
	static std::string toString(int);
	static std::string toString(unsigned int);
	static std::string toString(float);
	static std::string toString(sf::Uint16);

	static std::string toString(const sf::Vector2f&);
	static std::string toString(const sf::Vector2i&);
	static std::string toString(const sf::Vector2u&);

	static float distanceSquared(const sf::Vector2f&, const sf::Vector2f&);
	static int distanceSquared(const sf::Vector2i&, const sf::Vector2i&);

	static sf::Vector2f getDirection(const sf::Vector2f& pos, const sf::Vector2f& target);

	static bool consoleYesNo(const std::string& prompt);
};

#endif