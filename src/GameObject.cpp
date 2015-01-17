#include "GameObject.h"
#include "Game.h"

GameObject::GameObject()
{
	init();
}

GameObject::~GameObject()
{
	delete _controller;

	while (!_commandQueue.empty())
	{
		delete _commandQueue.back();
		_commandQueue.pop_back();
	}
}

void GameObject::init()
{
	_id = Game::_gameObjectManager.nextId();

	_controller = NULL;
	_speed = Constants::DEFAULT_OBJECT_SPEED;
	_hp = 1;
}

void GameObject::update()
{
	if (_hp <= 0)
		Game::_gameObjectManager.remove(_id);
	else
	{
		if (_controller)
			_controller->update(*this);

		while (!_commandQueue.empty())
		{
			_commandQueue.back()->execute(*this);
			delete _commandQueue.back();
			_commandQueue.pop_back();
		}
	}
}

void GameObject::destroy()
{
}

void GameObject::updateSprite()
{
	Game::getSpriteManager().updateSprite(_id, _pos);
}

void GameObject::addCommand(Command* command)
{
	_commandQueue.push_back(command);
}

GameObject::CollisionType GameObject::collisionResponse() const
{
	return COL_OTHER;
}

GameObject& GameObject::setTexture(const std::string& name)
{
	sf::Texture sizeFinder;
	sizeFinder.loadFromFile(std::string("../assets/sprites/").append(name).append(".png"));
	setSize(sf::Vector2f(sizeFinder.getSize()));

	_textureName = name;

	Game::getSpriteManager().registerSprite(_id, _textureName, _pos);

	return *this;
}

GameObject& GameObject::setController(Controller* controller)
{
	delete _controller;
	_controller = controller;

	return *this;
}

GameObject& GameObject::setPos(sf::Vector2f pos)
{
	_pos = pos;

	return *this;
}

GameObject& GameObject::setSize(const sf::Vector2f& size)
{
	_size = size;

	return *this;
}

GameObject& GameObject::setSpeed(float speed)
{
	_speed = speed;

	return *this;
}

GameObject& GameObject::setHp(sf::Int32 hp)
{
	_hp = hp;

	return *this;
}

void GameObject::move(float dx, float dy)
{
	updatePos(sf::Vector2f(_pos.x + dx, _pos.y + dy));
}

void GameObject::move(const sf::Vector2f& d)
{
	move(d.x, d.y);
}

void GameObject::updatePos(sf::Vector2f pos)
{
	bool update = _pos != pos;

	_pos = pos;

	if (update)
		updateSprite();
}

void GameObject::addHp(sf::Int32 delta)
{
	_hp += delta;
}

sf::Uint32 GameObject::getId() const
{
	return _id;
}

const std::string& GameObject::getTextureName() const
{
	return _textureName;
}

Controller* GameObject::getController() const
{
	return _controller;
}

const sf::Vector2f& GameObject::getPos() const
{
	return _pos;
}

const sf::Vector2f& GameObject::getSize() const
{
	return _size;
}

float GameObject::getSpeed() const
{
	return _speed;
}

sf::Int32 GameObject::getHp() const
{
	return _hp;
}

sf::FloatRect GameObject::getBoundingBox() const
{
	return sf::FloatRect(_pos, _size);
}

bool GameObject::offScreen() const
{
	bool offScreen = true;

	auto itr = Game::_gameObjectManager.playerBegin();

	while (offScreen && itr != Game::_gameObjectManager.playerEnd())
	{
		if (Helpful::distanceSquared(_pos, Game::_gameObjectManager.get(itr->second)->getPos())
			< Constants::OFFSCREEN_DISTANCE_SQRD)
			offScreen = false;

		itr++;
	}

	return offScreen;
}