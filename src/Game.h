#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "InputHandler.h"
#include "GameObjectManager.h"
#include "NetworkManager.h"
#include "SpawnManager.h"
#include "SpriteManager.h"
#include "ViewManager.h"
#include "WindowManager.h"
#include "World.h"

class Game
{
public:
	static void start();

	enum GameType
	{
		TYPE_SINGLE,
		TYPE_CLIENT,
		TYPE_SERVER,
		TYPE_NULL
	};
	static GameType getGameType();
	static sf::Uint16 getGameId();

	static void setGameId(sf::Uint16);

	static NetworkManager& getNetworkManager();
	static SpawnManager& getSpawnManager();
	static SpriteManager& getSpriteManager();
	static ViewManager& getViewManager();
	static const WindowManager& getWindowManager();

	static GameObjectManager _gameObjectManager;

private:
	static void gameLoop();
	static void setGameType();

	static InputHandler _inputHandler;
	static NetworkManager _networkManager;
	static SpawnManager _spawnManager;
	static SpriteManager _spriteManager;
	static ViewManager _viewManager;
	static WindowManager _windowManager;
	static World _background;

	static GameType _gameType;
	static sf::Uint16 _gameId;
};

#endif