#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

#include "GameObject.h"

class GameObjectManager
{
public:
	GameObjectManager();

	void updateAll();

	sf::Uint32 add(GameObject*);
	void remove(sf::Uint32 id);
	GameObject* get(sf::Uint32 id) const;

	sf::Uint32 nextId();
	sf::Uint32 peekId() const;
	const std::map<sf::Uint32, GameObject*>::const_iterator begin() const;
	const std::map<sf::Uint32, GameObject*>::const_iterator end() const;

	void registerPlayer(sf::Uint16 gameId, sf::Uint32 objectId);
	void removePlayer(sf::Uint32 objectId);
	const std::map<sf::Uint16, sf::Uint32>::const_iterator getPlayer(sf::Uint16 gameId) const;
	const std::map<sf::Uint16, sf::Uint32>::const_iterator playerBegin() const;
	const std::map<sf::Uint16, sf::Uint32>::const_iterator playerEnd() const;

private:
	void deleteObject(sf::Uint32 id);

	sf::Uint32 _currentId;

	std::map<sf::Uint32, GameObject*> _gameObjects;
	std::list<sf::Uint32> _removeQueue;
	std::map<sf::Uint16, sf::Uint32> _playerList;
};

#endif GAME_OBJECT_MANAGER_H