#pragma once

#include <cstdint>

class SimulationParameters
{
public:
	static constexpr uint8_t GridMaxAgentPerCell() { return 64; }
	static constexpr uint32_t GridCellSize() { return 256; }

	static float AgentRadius;
	static float RotationSpeed;
	static float MoveSpeed;
	static float MinAgentDistance;
};
