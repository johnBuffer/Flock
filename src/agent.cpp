#include "agent.hpp"
#include "simulation_parameters.hpp"

Agent::Agent(const up::Vec2& position, const up::Vec2& orientation)
	: m_position(position)
	, m_orientation(orientation)
{
}

void Agent::update(const std::vector<Agent*>& neighbors, const up::Vec2 & flock_center, float dt)
{
	m_position += (dt * SimulationParameters::MoveSpeed) * m_orientation;
}

void Agent::addToVertexArray(sf::VertexArray& va, uint64_t index) const
{
	constexpr float agent_length(16.0f);
	constexpr float agent_width(12.0f);

	const up::Vec2& normal(m_orientation.getNormal());

	const up::Vec2 head(m_position + agent_length * m_orientation);
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

const up::Vec2& Agent::getOrientation() const
{
	return m_orientation;
}
