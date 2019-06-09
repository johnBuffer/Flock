#pragma once

#include "vec2.hpp"
#include <vector>

class Agent
{
public:
	Agent(const up::Vec2& position, float orientation);

	void update(const std::vector<Agent*>& neighbors, const up::Vec2& flock_center, float dt);

	const up::Vec2& getPosition() const;

private:
	up::Vec2 m_position;
	float m_orientation;
};
