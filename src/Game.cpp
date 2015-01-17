#include "Game.h"
#include "Player.h"

void Game::start()
{
	setGameType();
	_networkManager.initialize();

	_viewManager.resetView(sf::FloatRect(0, 0, static_cast<float>(_windowManager.getWindow().getSize().x),
											   static_cast<float>(_windowManager.getWindow().getSize().y)));
	
	if (_gameType == TYPE_SINGLE)
	{
		Player* player = new Player;
		player->setHp(Constants::PLAYER_HP)
			.setTexture("376_");
		_gameObjectManager.add(player);

		_inputHandler.setObject(player->getId());
		_viewManager.setFollow(player->getId());
	}

	while (_windowManager.getWindow().isOpen())
	{
		gameLoop();
	}
}

void Game::gameLoop()
{
	sf::Event event;
	while (_windowManager.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Resized:
			std::cout << "resized: " << event.size.width << "x" << event.size.height << "\n";
			_viewManager.setSize(sf::Vector2f(sf::Vector2u(event.size.width, event.size.height)));
			break;

		case sf::Event::MouseWheelMoved:
			_viewManager.zoom(-event.mouseWheel.delta);
			break;

		case sf::Event::KeyReleased:
			if (event.key.code == sf::Keyboard::F1) _networkManager.send();
			else if (event.key.code == sf::Keyboard::F2) _networkManager.receive();
			break;
		}
	}

	if (_gameType == TYPE_SERVER)
		_networkManager.update();

	if (_gameType != TYPE_SERVER)
		_inputHandler.handleInput();

	if (_gameType == TYPE_CLIENT)
		_networkManager.send();
	else if (_gameType == TYPE_SERVER)
		_networkManager.receive();

	if (_gameType != TYPE_CLIENT)
	{
		_spawnManager.update();

		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::F4))
			_gameObjectManager.updateAll();

		if (_gameType == TYPE_SERVER)
			_networkManager.send();
	}
	else
		_networkManager.receive();

	if (_gameType != TYPE_SERVER)
		_viewManager.update(_windowManager.getWindow());

	_windowManager.getWindow().clear(sf::Color::White);

	_background.draw(_windowManager.getWindow());
	
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
		_spriteManager.drawAll(_windowManager.getWindow());

	_windowManager.update();
}

Game::GameType Game::getGameType()
{
	return _gameType;
}

void Game::setGameId(sf::Uint16 gameId)
{
	_gameId = gameId;

	if (_gameType == TYPE_CLIENT)
		_windowManager.getWindow().setTitle(std::string("Client ").append(Helpful::toString(_gameId)));
	else if (_gameType == TYPE_SERVER)
		_windowManager.getWindow().setTitle(std::string("Server"));
}

sf::Uint16 Game::getGameId()
{
	return _gameId;
}

void Game::setGameType()
{
	if (Helpful::consoleYesNo("Play in multiplayer?"))
	{
		if (Helpful::consoleYesNo("Host server?"))
			_gameType = TYPE_SERVER;
		else
			_gameType = TYPE_CLIENT;
	}
	else
		_gameType = TYPE_SINGLE;
}

NetworkManager& Game::getNetworkManager()
{
	return _networkManager;
}

SpawnManager& Game::getSpawnManager()
{
	return _spawnManager;
}

SpriteManager& Game::getSpriteManager()
{
	return _spriteManager;
}

ViewManager& Game::getViewManager()
{
	return _viewManager;
}

const WindowManager& Game::getWindowManager()
{
	return _windowManager;
}

InputHandler Game::_inputHandler;
GameObjectManager Game::_gameObjectManager;
WindowManager Game::_windowManager;
NetworkManager Game::_networkManager;
SpawnManager Game::_spawnManager;
SpriteManager Game::_spriteManager;
ViewManager Game::_viewManager;
World Game::_background;

Game::GameType Game::_gameType;
sf::Uint16 Game::_gameId;