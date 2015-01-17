#include "SpawnManager.h"
#include "Game.h"
#include "Enemy.h"
#include "AIChase.h"

SpawnManager::SpawnManager()
{
	_mobCount = 0;
}

void SpawnManager::update()
{
	if (_mobCount < Constants::SPAWN_MOB_CAP && rand() % Constants::SPAWN_CHANCE == 0)
	{
		float x = static_cast<float>(rand() % 1000) / 1000.0f;
		float y = sqrt(1.0f - x);
		if (rand() % 2)
			x *= -1.0f;
		if (rand() % 2)
			y *= -1.0f;

		float distance = static_cast<float>(rand() % Constants::SPAWN_DISTANCE_MAX
												   + Constants::SPAWN_DISTANCE_MIN);
		sf::Vector2f position(Game::getViewManager().getView().getCenter().x + distance * x,
							  Game::getViewManager().getView().getCenter().y + distance * y);

		std::string type;
		switch (rand() % 5)
		{
		case 0: type = "114_"; break;
		case 1: type = "207_"; break;
		case 2: type = "361_"; break;
		case 3: type = "451_"; break;
		case 4: type = "559_"; break;
		}

		Enemy* enemy = new Enemy();
		enemy->setController(new AIChase)
			.setSpeed(Constants::ENEMY_SPEED)
			.setPos(position)
			.setTexture(type)
			.setHp(Constants::ENEMY_HP);
		Game::_gameObjectManager.add(enemy);

		_mobCount++;

		std::cout << "Spawned at " << Helpful::toString(position) << ", mobCount: " << _mobCount << std::endl;
	}
}

int SpawnManager::getMobCount() const
{
	return _mobCount;
}

void SpawnManager::decrementMobCount()
{
	_mobCount--;
}