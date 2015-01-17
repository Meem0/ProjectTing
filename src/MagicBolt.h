#include "GameObject.h"

class MagicBolt : public GameObject
{
public:
	MagicBolt();

	void update();
	GameObject::CollisionType collisionResponse() const;

	MagicBolt& setTarget(const sf::Vector2f&);
	MagicBolt& setDamage(int);
	MagicBolt& setCreator(sf::Uint32);

private:
	bool collisionCheck();
	void setVelocity();
	
	sf::Uint32 _creator;
	sf::Vector2f _velocity;
	int _damage;
	bool _isVelocitySet;
	int _offScreenTimer;
};