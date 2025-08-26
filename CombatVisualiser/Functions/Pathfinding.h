#pragma once

#include <vector>
#include <memory>
#include <functional>

namespace mk
{
	namespace algo
	{
		typedef float(*Heuristic)(float, float);
		/* 
		======================================
		ASTAR PATHFINDING
		====================================== 
		*/

		struct Node // Struct to be overidden during a graph
		{
			Node()			= default;
			virtual ~Node() = default;

			Node(const Node& other)					= delete;
			Node(Node&& other) noexcept				= delete;
			Node& operator=(const Node& other)		= delete;
			Node& operator=(Node&& other) noexcept	= delete;

			virtual std::unique_ptr<Node> Clone() const = 0;
		}; 

		struct Connection final
		{
			Node* fromNodePtr{};
			Node* toNodePtr{};
			float weight{};
		};

		struct Graph final
		{
			Graph() = default;

			Graph(const Graph& other);
			Graph(Graph&& other) noexcept = default;
			Graph& operator=(const Graph& other);
			Graph& operator=(Graph&& other) noexcept = default;

			Connection* AddConnection(size_t fromIdx, size_t toIdx, float weight) noexcept;
			bool RemoveConnection(size_t fromIdx, size_t toIdx) noexcept;

			std::vector<Connection> connections{};
			std::vector<std::unique_ptr<Node>> nodes{};
		};

		
		class AStar final
		{
		public:
			AStar(Graph&& graph, const Heuristic& heuristicFunc);

			AStar(const AStar& other)					= delete;
			AStar(AStar&& other) noexcept				= delete;
			AStar& operator=(const AStar& other)		= delete;
			AStar& operator=(AStar&& other) noexcept	= delete;

			void FindPath();
			const std::vector<Connection>& GetPath() const;
			Graph& GetGraph(); // Returns a reference to adjust the graph in the future

		private:
			Graph m_Graph{};
			const Heuristic m_HeuristicFunc{};
			std::vector<Connection> m_Path{};
		};
	}
}