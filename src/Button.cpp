#include "Button.h"

Button::Button()
{
	_isKey = true;
	_lastPress = false;
	_key = sf::Keyboard::Unknown;
}

Button::Button(sf::Keyboard::Key key)
{
	setButton(key);
}

Button::Button(sf::Mouse::Button mouseButton)
{
	setButton(mouseButton);
}

bool Button::isKeyPressed()
{
	if (_isKey)
		return sf::Keyboard::isKeyPressed(_key);
	else
		return sf::Mouse::isButtonPressed(_mouseButton);
}

bool Button::pressEvent()
{
	bool result = false;

	if (!_lastPress && isKeyPressed())
		result = true;

	_lastPress = isKeyPressed();

	return result;
}

bool Button::isKey()
{
	return _isKey;
}

void Button::setButton(sf::Keyboard::Key key)
{
	_isKey = true;
	_key = key;
}

void Button::setButton(sf::Mouse::Button mouseButton)
{
	_isKey = false;
	_mouseButton = mouseButton;
}