#ifndef CONTROLLER_H
#define CONTROLLER_H

class GameObject;

class Controller
{
public:
	virtual void update(GameObject&) = 0;
};

#endif