#ifndef VIEW_MANAGER_H
#define VIEW_MANAGER_H

#include "includes.h"
#include "GameObject.h"

class ViewManager
{
public:
	ViewManager();

	void resetView(const sf::FloatRect&);

	void update(sf::RenderWindow&);
	void setSize(const sf::Vector2f&);
	void setFollow(sf::Uint32);
	void zoom(int);

	const sf::View& getView() const;
	const sf::FloatRect getBorder() const;

private:
	void resetSize();
	void scrollUpdate();

	sf::View _view;

	sf::Uint32 _followId;
	sf::FloatRect _followBounds;

	sf::Vector2f _size;
	float _scrollMargin;
	int _currentZoom;
	float _zoomAmount;
	float _moveAmount;
	float _rotateAmount;

	bool _modified;
	bool _isFollowSet;
};

#endif