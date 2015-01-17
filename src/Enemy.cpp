#include "Enemy.h"
#include "Game.h"

Enemy::Enemy()
{
}

void Enemy::update()
{
	GameObject::update();
	checkDespawn();
}

void Enemy::destroy()
{
	Game::getSpawnManager().decrementMobCount();
	std::cout << "Despawned, mob count: " << Game::getSpawnManager().getMobCount() << std::endl;
}

GameObject::CollisionType Enemy::collisionResponse(CollisionType other)
{
	return COL_ENEMY;
}

bool Enemy::checkDespawn()
{
	sf::Vector2i viewCenter(Game::getViewManager().getView().getCenter());
	int x = static_cast<int>(_pos.x) - viewCenter.x;
	int y = static_cast<int>(_pos.y) - viewCenter.y;

	if (x * x + y * y > Constants::DESPAWN_DISTANCE * Constants::DESPAWN_DISTANCE)
	{
		Game::_gameObjectManager.remove(_id);
		return true;
	}

	return false;
}