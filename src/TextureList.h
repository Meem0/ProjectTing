#ifndef TEXTURE_STORAGE_H
#define TEXTURE_STORAGE_H

#include "includes.h"

class TextureList
{
public:
	TextureList() : _head(NULL), _tail(NULL) {}

	struct TextureHolder
	{
		TextureHolder() : instances(0) {}
		TextureHolder(const std::string& textureName) : name(textureName), instances(0) {}

		sf::Texture texture;
		std::string name;
		unsigned int instances;
	};

	TextureHolder* add(const std::string&);
	TextureHolder* addIfNew(const std::string&);
	void clearUnused();

	TextureHolder* find(const std::string&) const;

private:
	struct Node
	{
		Node() : next(NULL) {}

		TextureHolder data;
		Node* next;
	};
	Node* _head;
	Node* _tail;
};

#endif