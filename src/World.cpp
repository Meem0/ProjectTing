#include "World.h"
#include "Game.h"

World::World()
{
	initialize();
}

void World::initialize()
{
	sf::RectangleShape backgroundSquares[BACKGROUND_ROWS][BACKGROUND_COLS];

	for (int i = 0; i < BACKGROUND_ROWS; i++)
	{
		for (int j = 0; j < BACKGROUND_COLS; j++)
		{
			backgroundSquares[i][j].setSize(sf::Vector2f(sf::Vector2i(SQUARE_SIZE, SQUARE_SIZE)));

			backgroundSquares[i][j].setPosition(static_cast<float>(j * SQUARE_SIZE), static_cast<float>(i * SQUARE_SIZE));
		}
	}

	colourBackground(backgroundSquares);

	sf::RenderTexture backTexture;
	backTexture.create(BACKGROUND_COLS * SQUARE_SIZE, BACKGROUND_ROWS * SQUARE_SIZE);
	backTexture.clear(sf::Color::White);
	for (int i = 0; i < BACKGROUND_ROWS; i++)
		for (int j = 0; j < BACKGROUND_COLS; j++)
			backTexture.draw(backgroundSquares[i][j]);
	backTexture.display();
	_backgroundTexture = backTexture.getTexture();
	_backgroundSprite.setTexture(_backgroundTexture, true);
}

void World::draw(sf::RenderWindow& window)
{
	//input - (input % 50 + 50) % 50
	sf::IntRect viewBorder(Game::getViewManager().getBorder());
	sf::Vector2i firstTile(viewBorder.left - (viewBorder.left % CS + CS) % CS,
						   viewBorder.top  - (viewBorder.top  % RS + RS) % RS);

	_backgroundSprite.setPosition(static_cast<float>(firstTile.x), static_cast<float>(firstTile.y));
		window.draw(_backgroundSprite);

	for (int x = firstTile.x; x < viewBorder.left + viewBorder.width; x += CS)
	{
		for (int y = firstTile.y; y < viewBorder.top + viewBorder.height; y += RS)
		{
			_backgroundSprite.setPosition(static_cast<float>(x), static_cast<float>(y));
			window.draw(_backgroundSprite);
		}
	}
}

void World::colourBackground(sf::RectangleShape backgroundSquares[][BACKGROUND_COLS])
{
	srand(static_cast<unsigned int>(time(NULL)));

	for (int i = 0; i < BACKGROUND_ROWS; i++)
		for (int j = 0; j < BACKGROUND_COLS; j++)
			backgroundSquares[i][j].setFillColor(sf::Color(std::rand() % 200 + 30, std::rand() % 200 + 30, std::rand() % 200 + 30, 40));
}