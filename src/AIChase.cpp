#include "AIChase.h"
#include "Game.h"
#include "GameObject.h"
#include "CommandMove.h"

AIChase::AIChase()
{
	_chaseDistance = 360000.0f; // 600^2
}

void AIChase::update(GameObject& object)
{
	GameObject* target = checkRange(object);
	if (target)
		stepTowards(object, target);
}

GameObject* AIChase::checkRange(GameObject& object)
{
	float smallestDistance = _chaseDistance;
	sf::Vector2f playerPos;
	sf::Vector2f distanceVec;
	float distance;
	GameObject* result = NULL;

	for (auto itr = Game::_gameObjectManager.playerBegin();
		 itr != Game::_gameObjectManager.playerEnd(); itr++)
	{
		playerPos = Game::_gameObjectManager.get(itr->second)->getPos();
		distanceVec = sf::Vector2f(playerPos.x - object.getPos().x, playerPos.y - object.getPos().y);
		distance = distanceVec.x * distanceVec.x + distanceVec.y * distanceVec.y;
		if (distance < _chaseDistance && distance < smallestDistance)
		{
			smallestDistance = distance;
			result = Game::_gameObjectManager.get(itr->second);
		}
	}

	return result;
}

void AIChase::stepTowards(GameObject& object, GameObject* target)
{
	sf::Vector2f direction(Helpful::getDirection(object.getPos(), target->getPos()));

	object.addCommand(new CommandMove(direction.x, direction.y));
}