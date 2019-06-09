#pragma once

#include "agent.hpp"
#include "access_grid.hpp"
#include "simulation_parameters.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

class Flock
{
public:
	using Grid = up::Grid<SimulationParameters::GridMaxAgentPerCell()>;
	using GridCell = up::GridCell<SimulationParameters::GridMaxAgentPerCell()>;

	Flock(const up::Vec2& dimension);

	void update(float dt);

	void draw(sf::RenderTarget& target) const;

	void addAgent(const up::Vec2& position, const up::Vec2& orientation);


private:
	std::vector<Agent> m_agents;
	Grid m_grid;
};
