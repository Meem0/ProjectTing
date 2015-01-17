#include "Player.h"

#include "Game.h"

Player::Player(sf::Uint16 gameId)
{
	Game::_gameObjectManager.registerPlayer(gameId, _id);
	_hp = Constants::PLAYER_HP;
}

/*void Player::destroy()
{
	Game::_gameObjectManager.removePlayer(_id);
}*/

GameObject::CollisionType Player::collisionResponse() const
{
	return COL_PLAYER;
}