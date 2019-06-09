#pragma once

#include <array>
#include <vector>
#include <swarm.hpp>

#include "simulation_parameters.hpp"
#include "agent.hpp"


namespace up
{
	// GridCell class
	template<int N>
	struct GridCell
	{
		GridCell()
			: item_count(0)
		{}

		GridCell(const GridCell<N>& cell)
			: item_count(cell.item_count)
			, items(cell.items)
		{}

		GridCell<N>& operator=(const GridCell<N>& cell)
		{
			item_count = cell.item_count;
			items = cell.items;

			return *this;
		}

		void add(Agent& a)
		{
			if (item_count < N) {
				items[item_count++] = &a;
			}
		}

		void clear()
		{
			for (uint8_t i(N); i--;) {
				items[i] = nullptr;
			}

			item_count = 0;
		}

		std::vector<Agent*> getItems()
		{
			std::vector<Agent*> vec_items;
			for (uint32_t i(0); i < item_count; ++i) {
				vec_items.push_back(items[i]);
			}

			return vec_items;
		}

		std::array<Agent*, N> items;
		uint8_t item_count;
	};


	// CellRegister class
	template<uint8_t N>
	struct CellRegister
	{
		CellRegister() :
			size(0)
		{}

		void init(uint32_t max_size)
		{
			cells.resize(max_size);
		}

		void add(GridCell<N>& gc)
		{
			cells[size++] = &gc;
		}

		void clear()
		{
			for (uint32_t i(0); i < size; ++i) {
				cells[i]->clear();
			}
			size = 0;
		}

		typename std::vector<GridCell<N>*>::iterator begin()
		{
			return cells.begin();
		}

		typename std::vector<GridCell<N>*>::iterator end()
		{
			return cells.begin() + size;
		}

		uint32_t size;
		std::vector<GridCell<N>*> cells;
	};


	// Grid class
	template<uint8_t N>
	class Grid
	{
	public:
		Grid(const Vec2& dimension, uint32_t cell_size, std::vector<Agent>& flock)
			: m_cell_size(cell_size)
			, m_width(uint32_t(dimension.x) / cell_size + 10)
			, m_height(uint32_t(dimension.y) / cell_size + 10)
			, m_non_empty()
			, m_swarm(flock, 1)
		{
			m_cells.resize(m_width * m_height);

			m_swarm.setJob([this](std::vector<Agent>& data, uint32_t id, uint32_t step) {addFlockSwarm(data, id, step); });
		}

		void addToCell(uint32_t grid_cell_x, uint32_t grid_cell_y, Agent& a)
		{
			GridCell<N>& current_cell = m_cells[grid_cell_y + m_height * grid_cell_x];

			current_cell.add(a);
		}

		GridCell<N>* getColliders(const Agent& a)
		{
			uint32_t body_x = uint32_t(a.getPosition().x + 1.0f);
			uint32_t body_y = uint32_t(a.getPosition().y + 1.0f);

			uint32_t grid_x = body_x / m_cell_size + 5;
			uint32_t grid_y = body_y / m_cell_size + 5;

			if (grid_x > 0 && grid_x < m_width &&
				grid_y > 0 && grid_y < m_height) {
				return &m_cells[grid_x + m_width * grid_y];
			}

			return nullptr;
		}

		void update()
		{
			clear();
			m_swarm.notifyReady();
			m_swarm.waitProcessed();
		}

		void addFlockSwarm(std::vector<Agent>& data, uint32_t id, uint32_t step)
		{
			const std::size_t size(data.size());
			for (std::size_t i(id); i < size; i += step) {
				add(data[i]);
			}
		}

		void vec2ToGridCoord(const Vec2& v, uint32_t& grid_x, uint32_t& grid_y)
		{
			int32_t body_x = int32_t(v.x);
			int32_t body_y = int32_t(v.y);

			grid_x = body_x / m_cell_size + 5;
			grid_y = body_y / m_cell_size + 5;
		}

		void add(Agent& a)
		{
			const float radius(SimulationParameters::AgentRadius);
			const Vec2& position(a.getPosition());
			
			uint32_t grid_x, grid_y;
			vec2ToGridCoord(position, grid_x, grid_y);

			const float grid_left((float(grid_x) - 5.0f)*m_cell_size);
			const float grid_right((float(grid_x) - 5.0f + 1.0f)*m_cell_size);
			const float grid_top((float(grid_x) - 5.0f)*m_cell_size);
			const float grid_bot((float(grid_x) - 5.0f + 1.0f)*m_cell_size);

			addToCell(grid_x, grid_y, a);

			const float delta_top(position.y - grid_top);
			const float delta_bot(grid_bot - position.y);

			if (position.x - grid_left < radius) {
				addToCell(grid_x - 1, grid_y, a);
				if (delta_top < radius) {
					addToCell(grid_x - 1, grid_y - 1, a);
					addToCell(grid_x, grid_y - 1, a);
				} else if (delta_bot) {
					addToCell(grid_x - 1, grid_y + 1, a);
					addToCell(grid_x, grid_y + 1, a);
				}
			} else if (grid_right - position.x < radius) {
				addToCell(grid_x + 1, grid_y, a);
				if (delta_top < radius) {
					addToCell(grid_x + 1, grid_y - 1, a);
					addToCell(grid_x, grid_y - 1, a);
				} else if (delta_bot < radius) {
					addToCell(grid_x + 1, grid_y + 1, a);
					addToCell(grid_x, grid_y + 1, a);
				}
			} else if (position.y - grid_top < radius) {
				addToCell(grid_x, grid_y - 1, a);
			} else if (grid_bot - position.y < radius) {
				addToCell(grid_x, grid_y + 1, a);
			}
		}

		CellRegister<N>& nonEmpty()
		{
			return m_non_empty;
		}

		void clear()
		{
			for (GridCell<N>& cell : m_cells) {
				cell.clear();
			}
		}

		std::vector<GridCell<N>>& getCells()
		{
			return m_cells;
		}

	private:
		uint32_t m_cell_size;
		uint32_t m_width;
		uint32_t m_height;

		std::vector<GridCell<N>> m_cells;
		CellRegister<N> m_non_empty;
		Swarm<Agent> m_swarm;
	};


}
