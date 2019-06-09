#pragma once

#include "agent.hpp"
#include "access_grid.hpp"
#include "simulation_parameters.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

class Flock
{
public:
	Flock(const uint32_t agent_count);

	void update(float dt);

	void draw(sf::RenderTarget& target) const;

	void addAgent(const up::Vec2& position, float orientation);

private:
	std::vector<Agent> m_agents;
	up::Grid<SimulationParameters::GridMaxAgentPerCell> m_grid;
};
