#include "TextureList.h"

TextureList::TextureHolder* TextureList::add(const std::string& textureName)
{
	if (!_head)
	{
		_head = new Node;
		_head->data.name = textureName;
		_tail = _head;
	}
	else
	{
		_tail->next = new Node;
		_tail = _tail->next;
		_tail->data.name = textureName;
	}

	if (!_tail->data.texture.loadFromFile(
		std::string("../assets/sprites/").append(textureName).append(".png")))
	{
		std::cout << "Couldn't load " << textureName << ".png\n";
		return NULL;
	}

	return &(_tail->data);
}

TextureList::TextureHolder* TextureList::addIfNew(const std::string& textureName)
{
	TextureHolder* result = find(textureName);

	if (result)
		return result;
	else
		return add(textureName);
}

void TextureList::clearUnused()
{
	int deleted = 0;

	Node* tempDelete;

	while (_head != NULL && _head->data.instances == 0)
	{
		tempDelete = _head;
		_head = _head->next;
		delete tempDelete;
		deleted++;
	}

	if (_head == NULL)
		return;

	Node* current = _head;

	while (current->next != NULL)
	{
		if (current->next->data.instances == 0)
		{
			tempDelete = current->next;
			current->next = current->next->next;
			delete tempDelete;
			deleted++;
		}
		else
			current = current->next;
	}

	std::cout << "deleted " << deleted << "\n";
}

TextureList::TextureHolder* TextureList::find(const std::string& textureName) const
{
	Node* current = _head;
	TextureHolder* result = NULL;
	bool found = false;

	while (!found && current != NULL)
	{
		if (textureName == current->data.name)
		{
			found = true;
			result = &(current->data);
		}

		current = current->next;
	}

	return result;
}