#ifndef SOLVER_H
#define SOLVER_H

#include <cmath>
#include <iostream>
#include <vector>

#include "math.hpp"
#include "particle.hpp"

class Solver {
   public:
	Solver() = default;

	Particle& addObject(sf::Vector2f position, float radius) {
		int gridx = position.x / NUM_GRIDS;
		int gridy = position.y / NUM_GRIDS;
		Particle newParticle(position, radius, gridx, gridy, objects.size());
		grid[gridx][gridy].push_back(newParticle);
		return objects.emplace_back(newParticle);
	}

	void setObjectVelocity(Particle& object, sf::Vector2f v) {
		object.setVelocity(v, step_dt / sub_steps);
	}

	void mousePull(sf::Vector2f pos) {
		for (Particle& obj : objects) {
			sf::Vector2f dir = pos - obj.pos;
			float dist = sqrt(dir.x * dir.x + dir.y * dir.y);
			obj.accelerate(dir * std::max(0.0f, 10 * (120 - dist)));
		}
	}

	void mousePush(sf::Vector2f pos) {
		for (Particle& obj : objects) {
			sf::Vector2f dir = pos - obj.pos;
			float dist = sqrt(dir.x * dir.x + dir.y * dir.y);
			obj.accelerate(dir * std::min(0.0f, -10 * (120 - dist)));
		}
	}

	void update(sf::Vector2u window_size) {
		time += step_dt;
		float substep_dt = step_dt / sub_steps;
		for (unsigned int i = 0; i < sub_steps; i++) {
			applyGravity();
			applyBoundary();
			updateObjects(substep_dt);
			checkCollisions(window_size);
		}
	}

	void setBoundary(sf::Vector2f pos, float radius) {
		boundary_center = pos;
		boundary_radius = radius;
	}

	std::vector<Particle> getObjects() const { return objects; }

	sf::Vector3f getBoundary() const {
		return {boundary_center.x, boundary_center.y, boundary_radius};
	}

	unsigned int getObjectsCount() const { return objects.size(); }

	float getTime() const { return time; }

   private:
	float time = 0.0f;
	std::vector<Particle> objects;
	std::vector<Particle> grid[100][100];
	sf::Vector2f gravity = {0.0f, 1000.0f};

	float step_dt = 1.0f / 60.0f;
	sf::Vector2f boundary_center;
	unsigned int sub_steps = 8;
	float boundary_radius;

	void applyGravity() {
		for (Particle& obj : objects) {
			obj.accelerate(gravity);
		}
	}

	void applyBoundary() {
		for (auto& obj : objects) {
			const sf::Vector2f v = boundary_center - obj.pos;
			const float dist = Math::magnitude(v);
			if (dist > (boundary_radius - obj.radius)) {
				const sf::Vector2f n = v / dist;
				const sf::Vector2f vel = obj.getVelocity();
				const sf::Vector2f perp = {-n.y, n.x};
				obj.pos = boundary_center - n * (boundary_radius - obj.radius);
				obj.setVelocity(vel - 2 * Math::dot_mag(vel, perp) * perp, -1);
			}
		}
	}

	std::vector<int> getCollisionParticles(int particleID, sf::Vector2u window_size) {
		Particle p = objects[particleID];
		std::vector<int> res;
		for (int i = p.gridx - 1; i <= p.gridx + 1; i++)
			for (int j = p.gridy - 1; j <= p.gridy + 1; j++) {
				if (i < 0 || j < 0 || i >= window_size.x / NUM_GRIDS ||
					j >= window_size.y / NUM_GRIDS)
					continue;
				for (Particle& pp : grid[i][j])
					if (pp.id != p.id) res.push_back(pp.id);
			}
		return res;
	}

	void checkCollisions(sf::Vector2u window_size) {
		int num_objects = objects.size();
		for (Particle& obj_1 : objects) {
			for (int i : getCollisionParticles(obj_1.id, window_size)) {
				Particle& obj_2 = objects[i];
				sf::Vector2f v = obj_1.pos - obj_2.pos;
				float dist = Math::magnitude(v);
				float min_dist = obj_1.radius + obj_2.radius;
				if (dist < min_dist) {
					sf::Vector2f n = v / dist;	// Normalize
					float total_mass = obj_1.radius * obj_1.radius + obj_2.radius * obj_2.radius;
					float mass_ratio = (obj_1.radius * obj_1.radius) / total_mass;
					float delta = 0.5f * (min_dist - dist);
					// Larger particle moves less
					obj_1.pos += n * (1 - mass_ratio) * delta;
					obj_2.pos -= n * mass_ratio * delta;
				}
			}
		}
	}

	void updateObjects(float dt) {
		for (int i = 0; i < 100; i++)
			for (int j = 0; j < 100; j++) grid[i][j].clear();
		for (auto& obj : objects) {
			obj.update(dt);
			grid[obj.gridx][obj.gridy].push_back(obj);
		}
	}
};

#endif
