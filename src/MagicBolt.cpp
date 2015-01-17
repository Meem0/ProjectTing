#include "MagicBolt.h"
#include "Game.h"

MagicBolt::MagicBolt()
{
	_controller = NULL;
	_isVelocitySet = false;
	_offScreenTimer = 0;

	_speed = Constants::MAGIC_BOLT_SPEED;
}

void MagicBolt::update()
{
	if (!_isVelocitySet)
		setVelocity();

	move(_velocity.x, _velocity.y);

	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::F6))
		collisionCheck();

	if (offScreen())
	{
		if (Constants::MAGIC_BOLT_OFFSCREEN_TIME < _offScreenTimer++)
			Game::_gameObjectManager.remove(_id);
	}
	else
		_offScreenTimer = 0;
}

GameObject::CollisionType MagicBolt::collisionResponse() const
{
	return COL_PLAYER_PROJECTILE;
}

MagicBolt& MagicBolt::setTarget(const sf::Vector2f& target)
{
	// temporarily store target in velocity until the first update calls setVelocity
	_velocity = target;

	return *this;
}

MagicBolt& MagicBolt::setDamage(int damage)
{
	_damage = damage;

	return *this;
}

MagicBolt& MagicBolt::setCreator(sf::Uint32 creator)
{
	_creator = creator;

	return *this;
}

void MagicBolt::setVelocity()
{
	sf::Vector2f normDir(Helpful::getDirection(_pos, _velocity));

	_velocity.x = normDir.x * _speed;
	_velocity.y = normDir.y * _speed;

	_isVelocitySet = true;
}

bool MagicBolt::collisionCheck()
{
	for (auto itr = Game::_gameObjectManager.begin();
		 itr != Game::_gameObjectManager.end(); itr++)
	{
		if (itr->second != this &&
			itr->second->collisionResponse() == COL_ENEMY &&
			itr->second->getId() != _creator &&
			getBoundingBox().intersects(itr->second->getBoundingBox()))
		{
			itr->second->addHp(-_damage);
			Game::_gameObjectManager.remove(_id);
			return true;
		}
	}

	return false;
}