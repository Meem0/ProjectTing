#include "CommandMove.h"
#include "CommandShoot.h"

sf::Packet& operator<<(sf::Packet& packet, Command*& command)
{
	sf::Packet& result = command->packetInsertion(packet);
	delete command;
	return result;
}

sf::Packet& operator>>(sf::Packet& packet, Command*& command)
{
	sf::Uint8 commandType;
	packet >> commandType;

	switch (commandType)
	{
	case sf::Uint8(0): command = new CommandMove; break;
	case sf::Uint8(1): command = new CommandShoot; break;
	default: std::cout << "Unknown command type received\n";
	}

	sf::Packet& result = command->packetExtraction(packet);
	return result;
}