#include "ViewManager.h"
#include "Game.h"

ViewManager::ViewManager()
{
	_scrollMargin = 0.3f;
	_currentZoom = 0;
	_zoomAmount = 0.2f;
	_moveAmount = 10.0f;
	_rotateAmount = 1.0f;
	_modified = false;
	_isFollowSet = false;
}

void ViewManager::resetView(const sf::FloatRect& rectangle)
{
	_view.reset(rectangle);
	_size = sf::Vector2f(rectangle.width, rectangle.height);
	_modified = true;
}

void ViewManager::update(sf::RenderWindow& window)
{
	if (!_isFollowSet)
		return;

	const sf::Sprite* followSprite = Game::getSpriteManager().getSprite(_followId);

	if (followSprite)
	{
		sf::Vector2f followPos(followSprite->getPosition());
		if (followPos != sf::Vector2f(_followBounds.left, _followBounds.top))
		{
			_followBounds.left = followPos.x;
			_followBounds.top = followPos.y;
			scrollUpdate();
		}
	}
	else
	{
		std::cout << "Tried to update view, but got a null follow sprite\n";
		_isFollowSet = false;
	}

	if (Game::getWindowManager().getWindowFocus())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			_view.move(-_moveAmount, 0);
			_modified = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			_view.move(_moveAmount, 0);
			_modified = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			_view.move(0, -_moveAmount);
			_modified = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			_view.move(0, _moveAmount);
			_modified = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Comma))
		{
			_view.rotate(-_rotateAmount);
			_modified = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period))
		{
			_view.rotate(_rotateAmount);
			_modified = true;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Slash))
		{
			_view.setRotation(0);
			_currentZoom = 0;
			resetSize();

			if (followSprite)
				scrollUpdate();
		}
	}

	if (_modified)
	{
		window.setView(_view);
		_modified = false;
	}
}

void ViewManager::setSize(const sf::Vector2f& newSize)
{
	_size = newSize;
	resetSize();
}

void ViewManager::setFollow(sf::Uint32 followId)
{
	std::cout << "setFollow id: " << followId << '\n';
	_followId = followId;
	const sf::Sprite* followSprite = Game::getSpriteManager().getSprite(_followId);

	if (followSprite)
	{
		_followBounds = followSprite->getGlobalBounds();
		_view.setCenter(sf::Vector2f(_followBounds.left + _followBounds.width  / 2,
									 _followBounds.top  + _followBounds.height / 2));
		_isFollowSet = true;
		_modified = true;
		scrollUpdate();
	}
	else
	{
		std::cout << "Tried to set a sprite to follow, but the sprite came out null.\n";
	}
}

void ViewManager::zoom(int factor)
{
	_currentZoom += factor;
	resetSize();
}

const sf::View& ViewManager::getView() const
{
	return _view;
}

const sf::FloatRect ViewManager::getBorder() const
{
	return sf::FloatRect(_view.getCenter().x - _view.getSize().x / 2, _view.getCenter().y - _view.getSize().y / 2,
						 _view.getSize().x, _view.getSize().y);
}

void ViewManager::resetSize()
{
	_view.setSize(_size.x + _currentZoom * _zoomAmount * _size.x,
				  _size.y + _currentZoom * _zoomAmount * _size.y);
	_modified = true;
}

void ViewManager::scrollUpdate()
{
	sf::FloatRect border = getBorder();

	float hMargin = border.width * _scrollMargin;
	float scrollLeft = border.left + hMargin;
	float scrollRight = border.left + border.width - hMargin;
	float vMargin = border.height * _scrollMargin;
	float scrollTop = border.top + vMargin;
	float scrollBottom = border.top + border.height - vMargin;

	float followRight = _followBounds.left + _followBounds.width;
	float followBottom = _followBounds.top + _followBounds.height;

	sf::Vector2f moveAmount(0.0f, 0.0f);
	bool change = false;

	if (_followBounds.left < scrollLeft)
	{
		moveAmount.x = _followBounds.left - scrollLeft;
		change = true;
	}
	else if (followRight > scrollRight)
	{
		moveAmount.x = followRight - scrollRight;
		change = true;
	}

	if (_followBounds.top < scrollTop)
	{
		moveAmount.y = _followBounds.top - scrollTop;
		change = true;
	}
	else if (followBottom > scrollBottom)
	{
		moveAmount.y = followBottom - scrollBottom;
		change = true;
	}

	if (change)
	{
		_modified = true;
		_view.move(moveAmount);
	}
}