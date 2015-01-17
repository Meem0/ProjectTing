#ifndef AI_CHASE_H
#define AI_CHASE_H

#include "includes.h"
#include "Controller.h"

class AIChase : public Controller
{
public:
	AIChase();

	void update(GameObject&);

private:
	GameObject* checkRange(GameObject&);
	void stepTowards(GameObject&, GameObject*);

	float _chaseDistance; // squared distance
};

#endif