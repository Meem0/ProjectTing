#include "CommandShoot.h"

#include "Game.h"
#include "MagicBolt.h"

void CommandShoot::execute(GameObject& object)
{
	MagicBolt* magicBolt = new MagicBolt();
	magicBolt->setCreator(object.getId())
		.setTarget(_target)
		.setDamage(Constants::MAGIC_BOLT_DAMAGE)
		.setPos(sf::Vector2f(object.getPos().x + object.getSize().x / 2,
				object.getPos().y + object.getSize().y / 2))
		.setTexture("magic_bolt");

	Game::_gameObjectManager.add(magicBolt);
}

sf::Packet& CommandShoot::packetInsertion(sf::Packet& packet) const
{
	return packet << sf::Uint8(1) << _target.x << _target.y;
}

sf::Packet& CommandShoot::packetExtraction(sf::Packet& packet)
{
	return packet >> _target.x >> _target.y;
}