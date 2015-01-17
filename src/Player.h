#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "Game.h"

class Player : public GameObject
{
public:
	Player(sf::Uint16 gameId = Game::getGameId());

	CollisionType collisionResponse() const;
};

#endif