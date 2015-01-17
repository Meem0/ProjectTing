#ifndef SPAWN_MANAGER_H
#define SPAWN_MANAGER_H

#include "includes.h"

class SpawnManager
{
public:
	SpawnManager();

	int getMobCount() const;

	void update();
	void decrementMobCount();

private:
	int _mobCount;
};

#endif