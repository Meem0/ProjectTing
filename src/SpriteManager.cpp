#include "SpriteManager.h"
#include "Game.h"

void SpriteManager::registerSprite(sf::Uint32 id, const std::string& textureName, const sf::Vector2f& pos = sf::Vector2f(0.0f, 0.0f))
{
	if (Game::getGameType() == Game::TYPE_SERVER)
	{
		Game::getNetworkManager().send(id, textureName, pos); 
		return;
	}

	if (_sprites.find(id) != _sprites.end())
	{
		std::cout << "tried to register a sprite with id " << id << ", but it already exists.\n";
		return;
	}

	TextureList::TextureHolder* textureHolder = _textureList.addIfNew(textureName);

	auto insertResult = _sprites.insert(std::make_pair(id, SpriteAndTexture(sf::Sprite(), textureHolder)));

	if (!insertResult.second)
		std::cout << "Failed to insert sprite with id " << id << "\n";
	else
	{
		insertResult.first->second.sprite.setTexture(textureHolder->texture);
		insertResult.first->second.sprite.setPosition(pos);
		textureHolder->instances++;
	}
}

void SpriteManager::removeSprite(sf::Uint32 id)
{
	if (Game::getGameType() == Game::TYPE_SERVER)
	{
		Game::getNetworkManager().send(id);
		return;
	}

	auto itr = _sprites.find(id);

	if (itr == _sprites.end())
		std::cout << "Tried to remove a sprite with id " << id << ", but it doesn't exist.\n";
	else
	{
		itr->second.texture->instances--;
		_sprites.erase(itr);
	}
}

void SpriteManager::updateSprite(sf::Uint32 id, const sf::Vector2f& pos)
{
	if (Game::getGameType() == Game::TYPE_SERVER)
	{
		Game::getNetworkManager().send(id, pos);
		return;
	}

	auto itr = _sprites.find(id);
	if (itr != _sprites.end())
	{
		sf::Vector2f floorPos(std::floor(pos.x), std::floor(pos.y));
		itr->second.sprite.setPosition(floorPos);
	}
	else
		std::cout << "Tried to update a sprite with id " << id << ", but it doesn't exist.\n";
}

const sf::Sprite* SpriteManager::getSprite(sf::Uint32 id) const
{
	auto itr = _sprites.find(id);

	if (itr != _sprites.end())
		return &(itr->second.sprite);
	else
		return NULL;
}

bool SpriteManager::offScreen(const sf::Sprite& sprite) const
{
	sf::FloatRect border = Game::getViewManager().getBorder();
	sf::Vector2f pos = sprite.getPosition();

	return (pos.x + sprite.getLocalBounds().width < border.left) || (pos.x > border.left + border.width) ||
		   (pos.y + sprite.getLocalBounds().width < border.top ) || (pos.y > border.top  + border.height);
}

bool SpriteManager::offScreen(sf::Uint32 id) const
{
	return offScreen(_sprites.at(id).sprite);
}

void SpriteManager::drawAll(sf::RenderWindow& window) const
{
	for (auto itr = _sprites.cbegin(); itr != _sprites.end(); itr++)
	{
		if (!offScreen(itr->second.sprite))
			window.draw(itr->second.sprite);
	}
}