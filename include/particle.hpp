#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/Graphics.hpp>

#define NUM_GRIDS 32

struct Particle {
   public:
	sf::Vector2f pos, prev_pos, acc;
	float radius;  // equivalent to mass in this case
	sf::Color color = sf::Color::Red;
	int gridx = 0, gridy = 0, id = 0;
	Particle() = default;

	Particle(sf::Vector2f position_, float radius_, int gridx_, int gridy_, int id_)
		: pos{position_},
		  prev_pos{position_},
		  acc{10.0f, 10.0f},
		  radius{radius_},
		  gridx{gridx_},
		  gridy{gridy_},
		  id{id_} {}

	// u is input/outside forces acting on object
	void update(float dt) {
		sf::Vector2f displacement = pos - prev_pos;
		prev_pos = pos;
		pos = pos + displacement + acc * (dt * dt);
		acc = {};
		gridx = pos.x / NUM_GRIDS;
		gridy = pos.y / NUM_GRIDS;
	}

	void accelerate(sf::Vector2f a) { acc += a; }

	void setVelocity(sf::Vector2f v, float dt) { prev_pos = pos - (v * dt); }

	void addVelocity(sf::Vector2f v, float dt) { prev_pos -= v * dt; }

	sf::Vector2f getVelocity() { return pos - prev_pos; }
};

#endif
