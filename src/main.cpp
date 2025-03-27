#include <SFML/Graphics.hpp>

#include "number_generator.hpp"
#include "renderer.hpp"
#include "solver.hpp"

static sf::Color getRainbow(float t) {
	const float r = sin(t);
	const float g = sin(t + 0.33f * 2.0f * Math::PI);
	const float b = sin(t + 0.66f * 2.0f * Math::PI);
	return {static_cast<uint8_t>(255.0f * r * r), static_cast<uint8_t>(255.0f * g * g),
			static_cast<uint8_t>(255.0f * b * b)};
}

int main() {
	unsigned int window_width = 800;
	unsigned int window_height = 800;

	auto window = sf::RenderWindow(sf::VideoMode({window_width, window_height}), "Physics Engine");
	window.setFramerateLimit(60);

	Solver solver;
	solver.setBoundary(sf::Vector2f(window_width / 2.0f, window_height / 2.0f),
					   (window_width - 20.0f) / 2.0f);

	// Set simulation attributes
	const float object_spawn_delay = 0.005f;
	const float object_spawn_speed = 1000.0f;
	const sf::Vector2f object_spawn_position = {window_width / 2.0f, window_height / 2.0f};
	const float object_min_radius = 5.0f;
	const float object_max_radius = 7.5f;
	const uint32_t max_objects_count = 2000;
	const float max_angle = Math::PI / 2;

	sf::Clock clock;

	Renderer renderer(window);

	while (window.isOpen()) {
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>() ||
				sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
				window.close();
			}
		}

		if (solver.getObjectsCount() < max_objects_count &&
			clock.getElapsedTime().asSeconds() >= object_spawn_delay) {
			clock.restart();
			auto& object = solver.addObject(object_spawn_position,
											RNGf::getRange(object_min_radius, object_max_radius));
			const float t = solver.getTime();
			const float angle = max_angle * sin(t) + Math::PI * 0.5f;
			solver.setObjectVelocity(object,
									 object_spawn_speed * sf::Vector2f{cos(angle), sin(angle)});
			object.color = getRainbow(t);
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			float ratio = window_width / window.getSize().x;
			sf::Vector2f pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)) * ratio;
			solver.mousePull(pos);
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
			float ratio = window_width / window.getSize().x;
			sf::Vector2f pos = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)) * ratio;
			solver.mousePush(pos);
		}

		solver.update(window.getSize());
		window.clear(sf::Color::White);
		renderer.render(solver);
		window.display();
	}
}
