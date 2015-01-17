#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include "includes.h"
#include "TextureList.h"

class SpriteManager
{
public:
	void registerSprite(sf::Uint32 id, const std::string& textureName,
		const sf::Vector2f& pos);
	void removeSprite(sf::Uint32 id);
	void updateSprite(sf::Uint32 id, const sf::Vector2f& pos);

	const sf::Sprite* getSprite(sf::Uint32 id) const;
	bool offScreen(const sf::Sprite&) const;
	bool offScreen(sf::Uint32 id) const;

	void drawAll(sf::RenderWindow&) const;

private:
	TextureList _textureList;

	struct SpriteAndTexture
	{
		SpriteAndTexture() : texture(NULL) { }
		SpriteAndTexture(sf::Sprite s, TextureList::TextureHolder* t) :
			sprite(s), texture(t) { }

		sf::Sprite sprite;
		TextureList::TextureHolder* texture;
	};
	std::unordered_map<sf::Uint32, SpriteAndTexture> _sprites;
};

#endif