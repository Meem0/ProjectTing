#include "GameObjectManager.h"
#include "Game.h"

GameObjectManager::GameObjectManager()
{
	_currentId = sf::Uint32(1);
}

void GameObjectManager::updateAll()
{
	for (auto itr = _gameObjects.begin(); itr != _gameObjects.end(); itr++)
	{
		itr->second->update();
	}

	while (!_removeQueue.empty())
	{
		deleteObject(_removeQueue.front());
		_removeQueue.pop_front();
	}
}

sf::Uint32 GameObjectManager::add(GameObject* object)
{
	sf::Uint32 id = object->getId();

	_gameObjects.insert(std::pair<const sf::Uint32, GameObject*>(id, object));

	return id;
}

void GameObjectManager::remove(sf::Uint32 id)
{
	_removeQueue.push_back(id);
}

GameObject* GameObjectManager::get(sf::Uint32 id) const
{
	// gets an iterator to the item at key
	auto itr = _gameObjects.find(id);

	// if key doesn't exist
	if (itr == _gameObjects.end())
	{
		std::cout << "Tried to get an object with ID " << id << ", but it doesn't exist.\n";
		return NULL;
	}
	return itr->second;
}

sf::Uint32 GameObjectManager::nextId()
{
	return _currentId++;
}

sf::Uint32 GameObjectManager::peekId() const
{
	return _currentId;
}

const std::map<sf::Uint32, GameObject*>::const_iterator GameObjectManager::begin() const
{
	return _gameObjects.cbegin();
}

const std::map<sf::Uint32, GameObject*>::const_iterator GameObjectManager::end() const
{
	return _gameObjects.cend();
}

void GameObjectManager::deleteObject(sf::Uint32 id)
{
	// gets an iterator to the item at key
	auto itr = _gameObjects.find(id);

	// if key exists
	if (itr != _gameObjects.end())
	{
		itr->second->destroy();

		Game::getSpriteManager().removeSprite(itr->second->getId());

		// delete the game object held at key in the map
		delete itr->second;

		//delete the entry from the map
		_gameObjects.erase(itr);
	}
	else
		std::cout << "Tried to remove a game object with ID "
				  << id << ", but it doesn't exist.\n";
}

void GameObjectManager::registerPlayer(sf::Uint16 gameId, sf::Uint32 objectId)
{
	_playerList.emplace(std::pair<sf::Uint16, sf::Uint32>(gameId, objectId));
}

void GameObjectManager::removePlayer(sf::Uint32 objectId)
{
	_playerList.erase(objectId);
}

const std::map<sf::Uint16, sf::Uint32>::const_iterator GameObjectManager::getPlayer(sf::Uint16 gameId) const
{
	return _playerList.find(gameId);
}

const std::map<sf::Uint16, sf::Uint32>::const_iterator GameObjectManager::playerBegin() const
{
	return _playerList.cbegin();
}

const std::map<sf::Uint16, sf::Uint32>::const_iterator GameObjectManager::playerEnd() const
{
	return _playerList.cend();
}