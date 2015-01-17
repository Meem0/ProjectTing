#include "Helpful.h"

std::string Helpful::toString(int input)
{
	return std::to_string(static_cast<long long>(input));
}

std::string Helpful::toString(unsigned int input)
{
	return std::to_string(static_cast<unsigned long long>(input));
}

std::string Helpful::toString(float input)
{
	return std::to_string(static_cast<long double>(input));
}

std::string Helpful::toString(sf::Uint16 input)
{
	return std::to_string(static_cast<unsigned long long>(input));
}

std::string Helpful::toString(const sf::Vector2f& input)
{
	return std::string("(").append(toString(input.x)).append(", ").append(toString(input.y)).append(")");
}

std::string Helpful::toString(const sf::Vector2i& input)
{
	return std::string("(").append(toString(input.x)).append(", ").append(toString(input.y)).append(")");
}

std::string Helpful::toString(const sf::Vector2u& input)
{
	return std::string("(").append(toString(input.x)).append(", ").append(toString(input.y)).append(")");
}

float Helpful::distanceSquared(const sf::Vector2f& a, const sf::Vector2f& b)
{
	float sideX = b.x - a.x;
	float sideY = b.y - a.y;

	return sideX * sideX + sideY * sideY;
}

int Helpful::distanceSquared(const sf::Vector2i& a, const sf::Vector2i& b)
{
	int sideX = b.x - a.x;
	int sideY = b.y - a.y;

	return sideX * sideX + sideY * sideY;
}

sf::Vector2f Helpful::getDirection(const sf::Vector2f& pos, const sf::Vector2f& target)
{
	if (pos == target)
		return sf::Vector2f(1.0f, 0.0f);

	sf::Vector2f direction(target.x - pos.x, target.y - pos.y);
	float invHyp = 1 / std::sqrt(direction.x * direction.x + direction.y * direction.y);

	return sf::Vector2f(direction.x * invHyp, direction.y * invHyp);
}

bool Helpful::consoleYesNo(const std::string& prompt)
{
	std::string input = "";
	bool result = false;

	while (input == "")
	{
		std::cout << prompt << " (y/n) ";
		std::cin >> input;

		if (!std::cin)
			std::cin.clear();
		else if (input.front() == 'Y' || input.front() == 'y')
			result = true;
		else if (input.front() == 'N' || input.front() == 'n')
			result = false;
		else
		{
			std::cout << "Invalid input, try again.\n";
			input = "";
		}
	}

	return result;
}