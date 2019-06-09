#include "flock.hpp"

#include <iostream>

Flock::Flock(const up::Vec2& dimension) :
	m_grid(dimension, SimulationParameters::GridCellSize(), m_agents)
{
}

void Flock::update(float dt)
{
	m_grid.update();

	for (GridCell& cell : m_grid.getCells()) {
		std::vector<Agent*> agents(cell.getItems());
		for (Agent* a : agents) {
			a->update(agents, up::Vec2(0.0f, 0.0f), dt);
		}
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

void Flock::addAgent(const up::Vec2& position, const up::Vec2& orientation)
{
	m_agents.emplace_back(position, orientation);
}

