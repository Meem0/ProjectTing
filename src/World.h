#ifndef WORLD_H
#define WORLD_H

#include "includes.h"

class World
{
private:
	static const int BACKGROUND_COLS = 12;
	static const int BACKGROUND_ROWS = 8;
	static const int SQUARE_SIZE = 100;

	static const int CS = BACKGROUND_COLS * SQUARE_SIZE;
	static const int RS = BACKGROUND_ROWS * SQUARE_SIZE;

	sf::Texture _backgroundTexture;
	sf::Sprite _backgroundSprite;

public:
	World();
	void initialize();
	void draw(sf::RenderWindow&);
	void colourBackground(sf::RectangleShape[][BACKGROUND_COLS]);
};

#endif