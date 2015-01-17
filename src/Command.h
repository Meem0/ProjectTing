#ifndef COMMAND_H
#define COMMAND_H

#include "includes.h"

class GameObject;

class Command
{
public:
	virtual void execute(GameObject&) = 0;

	friend sf::Packet& operator<<(sf::Packet& packet, Command*& command);
	friend sf::Packet& operator>>(sf::Packet& packet, Command*& command);

protected:
	/*
		0 - Move
		1 - Shoot
	*/	
	virtual sf::Packet& packetInsertion(sf::Packet&) const = 0;
	virtual sf::Packet& packetExtraction(sf::Packet&) = 0;
};

#endif