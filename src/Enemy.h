#ifndef ENEMY_H
#define ENEMY_H

#include "GameObject.h"

class Enemy : public GameObject
{
public:
	Enemy();

	void update();
	void destroy();
	CollisionType collisionResponse(CollisionType);

private:
	bool checkDespawn();
};

#endif