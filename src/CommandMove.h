#ifndef COMMAND_MOVE_H
#define COMMAND_MOVE_H

#include "Command.h"

class CommandMove : public Command
{
public:
	CommandMove() { }
	CommandMove(const sf::Vector2f& delta) : _delta(delta) { }
	CommandMove(float dx, float dy) : _delta(dx, dy) { }

	void execute(GameObject&);

protected:
	sf::Packet& packetInsertion(sf::Packet&) const;
	sf::Packet& packetExtraction(sf::Packet&);

private:
	sf::Vector2f _delta;
};

#endif