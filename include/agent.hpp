#pragma once

#include "vec2.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

class Agent
{
public:
	Agent(const up::Vec2& position, float orientation);

	void update(std::vector<Agent>& agents, const up::Vec2& dimension, float dt);

	void addToVertexArray(sf::VertexArray& va, uint64_t index) const;

	const up::Vec2& getPosition() const;

	up::Vec2 getOrientation() const;

	static up::Vec2 getCenterOf(const std::vector<Agent*>& agents);

private:
	up::Vec2 m_position;
	float m_orientation;

	void avoidBounds(const up::Vec2& bounds);
	std::vector<Agent*> getNeighbors(std::vector<Agent>& agents, float max_distance) const;
};
