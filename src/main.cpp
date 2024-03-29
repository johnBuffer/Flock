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

	Flock flock(up::Vec2(win_width, win_height));

	const uint32_t n(128);
	for (uint32_t i(0); i < n; ++i) {
		flock.addAgent(up::Vec2(rand()%win_width, rand() % win_height), rand()%314159 / 5000.0f);
	}

	bool run(false);

	sfev::EventManager event_handler(window);
	event_handler.addEventCallback(sf::Event::Closed, [&](const sf::Event&) {window.close(); });
	event_handler.addKeyPressedCallback(sf::Keyboard::Space, [&](const sf::Event&) {run = !run; });

	constexpr float dt(0.016f);

	while (window.isOpen())
	{
		const sf::Vector2i mouse_pos(sf::Mouse::getPosition(window));
		event_handler.processEvents();

		if (run) {
			flock.update(dt);
		}

		window.clear(sf::Color::Black);
		flock.draw(window);
		window.display();
	}

	return 0;
}