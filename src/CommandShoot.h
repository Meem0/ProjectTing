#ifndef COMMAND_SHOOT_H
#define COMMAND_SHOOT_H

#include "Command.h"

class CommandShoot : public Command
{
public:
	CommandShoot() { }
	CommandShoot(const sf::Vector2f& target) : _target(target) { }

	void execute(GameObject&);

protected:
	sf::Packet& packetInsertion(sf::Packet&) const;
	sf::Packet& packetExtraction(sf::Packet&);

private:
	sf::Vector2f _target;
};

#endif