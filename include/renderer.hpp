#ifndef RENDERER_H
#define RENDERER_H

#include <string>

#include "solver.hpp"

class Renderer {
   public:
	explicit Renderer(sf::RenderTarget& target_) : target{target_} {}

	void render(Solver& solver) const {
		target.clear(background_color);
		const sf::Vector3f boundary = solver.getBoundary();
		sf::CircleShape boundary_background(boundary.z);

		boundary_background.setOrigin({boundary.z, boundary.z});
		boundary_background.setPosition({boundary.x, boundary.y});
		boundary_background.setFillColor(sf::Color::Black);
		boundary_background.setPointCount(128);

		target.draw(boundary_background);

		// Draw objects
		sf::CircleShape circle{1.0f};
		circle.setPointCount(32);
		circle.setOrigin({1.0f, 1.0f});
		const auto& objects = solver.getObjects();
		for (const auto& obj : objects) {
			circle.setPosition(obj.pos);
			circle.setScale({obj.radius, obj.radius});
			circle.setFillColor(obj.color);
			target.draw(circle);
		}
	}

   private:
	sf::RenderTarget& target;
	const sf::Color background_color = sf::Color(80, 80, 60, 255);
};

#endif
