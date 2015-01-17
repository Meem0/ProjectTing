#include "WindowManager.h"
#include "Game.h"

WindowManager::WindowManager()
{
	_window.create(sf::VideoMode(1280, 720), "ProjectTing");
	_windowFocus = true;
	_window.setFramerateLimit(60);
	_options.limitedFramerate = true;
	_options.vsync = false;

	_debugInfo.font.loadFromFile("../assets/fonts/arial.ttf");
	_debugInfo.text.setFont(_debugInfo.font);
	_debugInfo.text.setPosition(32, 16);
	_debugInfo.text.setColor(sf::Color::Black);
	_debugInfo.text.setCharacterSize(10);
}

void WindowManager::update()
{
	drawDebug();
	_window.display();

	#if defined(SFML_SYSTEM_WINDOWS)
	if (!_windowFocus && sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		sf::Vector2u mousePos(sf::Mouse::getPosition(_window));

		if (mousePos.x >= 0 && mousePos.x <= _window.getSize().x &&
			mousePos.y >= 0 && mousePos.y <= _window.getSize().y)
			SetFocus(_window.getSystemHandle());
	}
	#endif
}

void WindowManager::toggleFramerateLimit()
{
	if (_options.limitedFramerate)
		_window.setFramerateLimit(0);
	else
		_window.setFramerateLimit(60);

	_options.limitedFramerate = !_options.limitedFramerate;
}

void WindowManager::toggleVSync()
{
	_options.vsync = !_options.vsync;
	_window.setVerticalSyncEnabled(_options.vsync);
}

sf::RenderWindow& WindowManager::getWindow()
{
	return _window;
}

const sf::RenderWindow& WindowManager::getCWindow() const
{
	return _window;
}

bool WindowManager::getWindowFocus() const
{
	return _windowFocus;
}

bool WindowManager::pollEvent(sf::Event& event)
{
	while (_window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			_window.close();
			break;

		case sf::Event::LostFocus:
			std::cout << "lost focus\n";
			_windowFocus = false;
			break;

		case sf::Event::GainedFocus:
			std::cout << "gained focus\n";
			_windowFocus = true;
			break;

		case sf::Event::KeyReleased:
			if (event.key.code == sf::Keyboard::F) toggleFramerateLimit();
			else if (event.key.code == sf::Keyboard::V) toggleVSync();
			else return true;
			break;

		default:
			return true;
		}
	}

	return false;
}

void WindowManager::drawDebug()
{
	if (_debugInfo.updateTime.getElapsedTime().asMilliseconds() >= 200)
	{
		_debugInfo.updateTime.restart();

		std::string text;

		sf::Int64 FPS = 1000000 / (_debugInfo.fps.getElapsedTime().asMicroseconds() + 1);
		text.append("FPS: ").append(std::to_string(FPS));
		
		if (_options.vsync)
			text.append("\nvsync enabled");
		else
			text.append("\nvsync disabled");

		sf::Vector2i mPixPos = sf::Mouse::getPosition(_window);
		text.append("\nMouse pixel position: ").append(Helpful::toString(mPixPos));
		text.append("\nMouse coord position: ").append(Helpful::toString(_window.mapPixelToCoords(mPixPos)));

		sf::IntRect viewport = _window.getViewport(Game::getViewManager().getView());
		text.append("\nWindow viewport size: ").append(Helpful::toString(viewport.width)).append("*").append(Helpful::toString(viewport.height));

		sf::FloatRect viewBorder = Game::getViewManager().getBorder();

		text.append("\nView origin: ").append(Helpful::toString(Game::getViewManager().getView().getCenter()))
			.append(" - top-left: ").append(Helpful::toString(sf::Vector2f(viewBorder.left, viewBorder.top)));

		text.append("\nView size: ").append(Helpful::toString(sf::Vector2f(viewBorder.width, viewBorder.height)))
			.append(" - ratio: ").append(Helpful::toString(viewBorder.width / viewBorder.height));

		_debugInfo.text.setString(text);
	}

	sf::View realView = _window.getView();
	_window.setView(_window.getDefaultView());

	_window.draw(_debugInfo.text);
	_debugInfo.fps.restart();

	_window.setView(realView);
}