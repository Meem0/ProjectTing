#ifndef BUTTON_H
#define BUTTON_H

#include "includes.h"

class Button
{
public:
	Button();
	Button(sf::Keyboard::Key);
	Button(sf::Mouse::Button);

	bool isKeyPressed();
	bool pressEvent();
	bool isKey();
	bool released();

	void setButton(sf::Keyboard::Key);
	void setButton(sf::Mouse::Button);

private:
	bool _isKey;
	bool _lastPress;
	sf::Keyboard::Key _key;
	sf::Mouse::Button _mouseButton;
};

#endif