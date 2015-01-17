#include "InputHandler.h"
#include "Game.h"
#include "CommandMove.h"
#include "CommandShoot.h"

InputHandler::InputHandler()
{
	setControls(Button(sf::Keyboard::W), Button(sf::Keyboard::S),
				Button(sf::Keyboard::A), Button(sf::Keyboard::D), Button(sf::Mouse::Left));
}

InputHandler::InputHandler(Button up, Button down, Button left, Button right, Button shoot)
{
	setControls(up, down, left, right, shoot);
}

void InputHandler::setControls(Button up, Button down, Button left, Button right, Button shoot)
{
	_ctrlUp = up;
	_ctrlDown = down;
	_ctrlLeft = left;
	_ctrlRight = right;
	_ctrlShoot = shoot;
}

void InputHandler::setObject(sf::Uint32 object)
{
	_object = object;
}

const float sqrtHalf = 0.707107f;

void InputHandler::handleInput()
{
	if (!Game::getWindowManager().getWindowFocus())
		return;

	float vertical = 0.0f, horizontal = 0.0f;
	bool up = _ctrlUp.isKeyPressed(), down = _ctrlDown.isKeyPressed(),
		 left = _ctrlLeft.isKeyPressed(), right = _ctrlRight.isKeyPressed();

	if (up && down)
		up = false;
	if (left && right)
		left = false;

	if (up)
	{
		if (left)
		{
			horizontal = -sqrtHalf;
			vertical = -sqrtHalf;
		}
		else if (right)
		{
			horizontal = sqrtHalf;
			vertical = -sqrtHalf;
		}
		else
			vertical = -1.0f;
	}
	else if (down)
	{
		if (left)
		{
			horizontal = -sqrtHalf;
			vertical = sqrtHalf;
		}
		else if (right)
		{
			horizontal = sqrtHalf;
			vertical = sqrtHalf;
		}
		else
			vertical = 1.0f;
	}
	else
	{
		if (left)
			horizontal = -1.0f;
		else if (right)
			horizontal = 1.0f;
	}

	bool move = horizontal || vertical, shoot = _ctrlShoot.pressEvent();
	GameObject* object = NULL;

	if (move || shoot)
	{
		if (Game::getGameType() == Game::TYPE_SINGLE)
			object = Game::_gameObjectManager.get(_object);

		if (move)
		{
			CommandMove* comMove = new CommandMove(horizontal, vertical);

			if (Game::getGameType() == Game::TYPE_SINGLE)
				object->addCommand(comMove);
			else
				Game::getNetworkManager().send(comMove);
		}

		if (shoot)
		{
			CommandShoot* comShoot = new CommandShoot(
					sf::Vector2f(Game::getWindowManager().getCWindow().mapPixelToCoords(
					sf::Mouse::getPosition(Game::getWindowManager().getCWindow()))));

			if (Game::getGameType() == Game::TYPE_SINGLE)
				object->addCommand(comShoot);
			else
				Game::getNetworkManager().send(comShoot);
		}
	}
}