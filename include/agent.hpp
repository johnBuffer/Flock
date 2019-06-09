#pragma once

#include "vec2.hpp"
#include <vector>
#include <SFML/Graphics.hpp>

class Agent
{
public:
	Agent(const up::Vec2& position, const up::Vec2& orientation);

	void update(const std::vector<Agent*>& neighbors, const up::Vec2& flock_center, float dt);

	void addToVertexArray(sf::VertexArray& va, uint64_t index) const;

	const up::Vec2& getPosition() const;

	const up::Vec2& getOrientation() const;

private:
	up::Vec2 m_position;
	up::Vec2 m_orientation;
};
