#include <iostream>

#include "flock.hpp"
#include "event_manager.hpp"

int main()
{
	const uint32_t win_width = 1600;
	const uint32_t win_height = 900;
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;

	sf::RenderWindow window(sf::VideoMode(win_width, win_height), "Flock", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);

	Flock flock(up::Vec2(2000.0f, 2000.0f));

	flock.addAgent(up::Vec2(500.0f, 500.0f), up::Vec2(0.0f, 1.0f));

	sfev::EventManager event_handler(window);
	event_handler.addEventCallback(sf::Event::Closed, [&](const sf::Event&) {window.close(); });

	constexpr float dt(0.016f);

	while (window.isOpen())
	{
		const sf::Vector2i mouse_pos(sf::Mouse::getPosition(window));
		event_handler.processEvents();

		flock.update(dt);

		window.clear(sf::Color::Black);
		flock.draw(window);
		window.display();
	}

	return 0;
}