#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "includes.h"
#include "Button.h"

class InputHandler
{
public:
	InputHandler();
	InputHandler(Button up, Button down, Button left, Button right, Button shoot);

	void setControls(Button up, Button down, Button left, Button right, Button shoot);
	void setObject(sf::Uint32);

	void handleInput();

private:
	sf::Uint32 _object;

	Button _ctrlUp;
	Button _ctrlDown;
	Button _ctrlLeft;
	Button _ctrlRight;
	Button _ctrlShoot;
};

#endif