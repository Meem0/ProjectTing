#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "includes.h"
#include "Controller.h"
#include "Command.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	virtual void update();
	virtual void destroy();
	void updateSprite();
	void addCommand(Command*);

	enum CollisionType
	{
		COL_PLAYER,
		COL_PLAYER_PROJECTILE,
		COL_ENEMY,
		COL_OTHER
	};
	virtual CollisionType collisionResponse() const;
	
	GameObject& setTexture(const std::string&);
	GameObject& setController(Controller*);
	GameObject& setPos(sf::Vector2f);
	GameObject& setSize(const sf::Vector2f&);
	GameObject& setSpeed(float);
	GameObject& setHp(sf::Int32);
	
	void move(float, float);
	void move(const sf::Vector2f&);
	void updatePos(sf::Vector2f);
	void addHp(sf::Int32);
	
	sf::Uint32 getId() const;
	const std::string& getTextureName() const;
	Controller* getController() const;

	const sf::Vector2f& getPos() const;
	const sf::Vector2f& getSize() const;
	float getSpeed() const;
	sf::Int32 getHp() const;

	sf::FloatRect getBoundingBox() const;
	bool offScreen() const;

protected:
	void init();

	const static int OFFSCREEN_DISTANCE_SQRD = 5290000; // 2300^2

	sf::Uint32 _id;
	std::string _textureName;

	Controller* _controller;
	std::vector<Command*> _commandQueue;

	sf::Vector2f _pos;
	sf::Vector2f _size;
	float _speed;
	sf::Int32 _hp;
};

#endif