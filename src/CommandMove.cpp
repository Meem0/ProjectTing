#include "CommandMove.h"
#include "GameObject.h"

void CommandMove::execute(GameObject& object)
{
	object.move(_delta.x * object.getSpeed(), _delta.y * object.getSpeed());
}

sf::Packet& CommandMove::packetInsertion(sf::Packet& packet) const
{
	return packet << sf::Uint8(0) << _delta.x << _delta.y;
}

sf::Packet& CommandMove::packetExtraction(sf::Packet& packet)
{
	return packet >> _delta.x >> _delta.y;
}