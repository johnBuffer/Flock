#include "flock.hpp"

#include <iostream>

Flock::Flock(const up::Vec2& dimension)
	: m_dimension(dimension)
	, m_grid(dimension, SimulationParameters::GridCellSize(), m_agents)
{
}

void Flock::update(float dt)
{
	const up::Vec2 center(getCenter());

	for (Agent& a : m_agents) {
		a.update(m_agents, m_dimension, dt);
	}
}

void Flock::draw(sf::RenderTarget& target) const
{
	const std::size_t size(m_agents.size());
	sf::VertexArray va(sf::Triangles, size*3);

	for (std::size_t i(0); i<size; ++i) {
		m_agents[i].addToVertexArray(va, i);
	}

	target.draw(va);
}

void Flock::addAgent(const up::Vec2& position, float orientation)
{
	m_agents.emplace_back(position, orientation);
}

up::Vec2 Flock::getCenter() const
{
	up::Vec2 center(0.0f, 0.0f);

	for (const Agent& a : m_agents) {
		center.x += a.getPosition().x;
		center.y += a.getPosition().y;
	}

	center.x /= float(m_agents.size());
	center.y /= float(m_agents.size());

	return center;
}

