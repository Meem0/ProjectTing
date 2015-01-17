#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include "includes.h"

class WindowManager
{
public:
	WindowManager();

	void update();

	struct DebugInfo
	{
		sf::Font font;
		sf::Text text;
		sf::Clock fps;
		sf::Clock updateTime;
	};
	DebugInfo _debugInfo;

	struct Options
	{
		bool limitedFramerate;
		bool vsync;
	};
	Options _options;
	void toggleFramerateLimit();
	void toggleVSync();

	sf::RenderWindow& getWindow();
	const sf::RenderWindow& getCWindow() const;
	bool getWindowFocus() const;
	bool pollEvent(sf::Event&);

private:
	sf::RenderWindow _window;
	bool _windowFocus;

	void drawDebug();
};

#endif