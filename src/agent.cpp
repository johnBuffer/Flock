#include "agent.hpp"
#include "simulation_parameters.hpp"

#include <iostream>
#include <cmath>

Agent::Agent(const up::Vec2& position, float orientation)
	: m_position(position)
	, m_orientation(orientation)
{
}

void Agent::update(std::vector<Agent>& agents, const up::Vec2& dimension, float dt)
{
	const up::Vec2 orientation(getOrientation());
	const up::Vec2 normal(orientation.getNormal());
	const float max_dist(64.0f);

	const float center_coef = 100.0f;
	const float cohesion_coef = 2.0f;
	const float repulsion_coef = 0.005f;

	std::vector<Agent*> neighbors(getNeighbors(agents, max_dist));
	if (!neighbors.empty()) {
		const up::Vec2 center(getCenterOf(neighbors));
		const up::Vec2 to_center((m_position - center).getNormalized());
		const float dot(to_center.dot(normal));
		m_orientation += center_coef * SimulationParameters::RotationSpeed * (dot > 0.0f ? -1.0f : 1.0f) * dt;

		float neighbors_orientation(0.0f);
		for (Agent* a : neighbors) {
			neighbors_orientation += a->m_orientation;
		}
		neighbors_orientation /= float(neighbors.size());
		m_orientation += (neighbors_orientation - m_orientation) * cohesion_coef * dt;

		for (Agent* a : neighbors) {
			const up::Vec2 to_other(a->m_position - m_position);
			const float distance(to_other.length());

			if (distance < SimulationParameters::MinAgentDistance) {
				const float delta(repulsion_coef * (distance - SimulationParameters::MinAgentDistance));
				const up::Vec2 rep(to_other.getNormalized());
				m_position += delta * rep;
			}
		}
	}

	//avoidBounds(dimension);
	
	if (m_position.x < 0.0f) {
		m_position.x = dimension.x;
	}

	if (m_position.x > dimension.x) {
		m_position.x = 0.0f;
	}

	if (m_position.y < 0.0f) {
		m_position.y = dimension.y;
	}

	if (m_position.y > dimension.y) {
		m_position.y = 0.0f;
	}

	m_position += (dt * SimulationParameters::MoveSpeed) * getOrientation();
}

void Agent::addToVertexArray(sf::VertexArray& va, uint64_t index) const
{
	constexpr float agent_length(16.0f);
	constexpr float agent_width(12.0f);

	const up::Vec2 orientation(getOrientation());
	const up::Vec2& normal(orientation.getNormal());

	const up::Vec2 head(m_position + agent_length * orientation);
	const up::Vec2 wng1(m_position + (0.5f * agent_width) * normal);
	const up::Vec2 wng2(m_position - (0.5f * agent_width) * normal);

	va[3 * index + 0].position = sf::Vector2f(head.x, head.y);
	va[3 * index + 1].position = sf::Vector2f(wng1.x, wng1.y);
	va[3 * index + 2].position = sf::Vector2f(wng2.x, wng2.y);

	va[3 * index + 0].color = sf::Color::Red;
	va[3 * index + 1].color = sf::Color::Red;
	va[3 * index + 2].color = sf::Color::Red;
}

const up::Vec2& Agent::getPosition() const
{
	return m_position;
}

up::Vec2 Agent::getOrientation() const
{
	return up::Vec2(cos(m_orientation), sin(m_orientation));
}

up::Vec2 Agent::getCenterOf(const std::vector<Agent*>& agents)
{
	up::Vec2 center(0.0f, 0.0f);

	for (const Agent* a : agents) {
		center.x += a->getPosition().x;
		center.y += a->getPosition().y;
	}

	center.x /= float(agents.size());
	center.y /= float(agents.size());

	return center;
}

void Agent::avoidBounds(const up::Vec2& bounds)
{
	const float min_dist(2.0f * SimulationParameters::MinAgentDistance);
	const up::Vec2 orientation(getOrientation());
	const float speed_factor(2.0f);

	if (m_position.y < min_dist) {
		if (orientation.y < 0.0f) {
			if (orientation.x > 0.0f) {
				m_orientation += speed_factor * SimulationParameters::RotationSpeed;
			} else {
				m_orientation -= speed_factor * SimulationParameters::RotationSpeed;
			}
		}
	}

	if (m_position.y > bounds.y - min_dist) {
		if (orientation.y > 0.0f) {
			if (orientation.x > 0.0f) {
				m_orientation -= speed_factor * SimulationParameters::RotationSpeed;
			} else {
				m_orientation += speed_factor * SimulationParameters::RotationSpeed;
			}
		}
	}

	if (m_position.x < min_dist) {
		if (orientation.x < 0.0f) {
			if (orientation.y > 0.0f) {
				m_orientation -= speed_factor * SimulationParameters::RotationSpeed;
			} else {
				m_orientation += speed_factor * SimulationParameters::RotationSpeed;
			}
		}
	}

	if (m_position.x > bounds.x - min_dist) {
		if (orientation.x > 0.0f) {
			if (orientation.y > 0.0f) {
				m_orientation += speed_factor * SimulationParameters::RotationSpeed;
			} else {
				m_orientation -= speed_factor * SimulationParameters::RotationSpeed;
			}
		}
	}
}

std::vector<Agent*> Agent::getNeighbors(std::vector<Agent>& agents, float max_distance) const
{
	std::vector<Agent*> neighbors;

	for (Agent& a : agents) {
		if (&a != this) {
			const up::Vec2 v(m_position - a.getPosition());
			const float distance(v.length());
			if (distance < max_distance) {
				neighbors.push_back(&a);
			}
		}
	}

	return neighbors;
}
