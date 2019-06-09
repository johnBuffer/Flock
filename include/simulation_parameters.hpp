#pragma once

#include <cstdint>

class SimulationParameters
{
public:
	static constexpr uint8_t GridMaxAgentPerCell = 64;

	static float AgentRadius;
	static float RotationSpeed;
	static float MoveSpeed;
};
