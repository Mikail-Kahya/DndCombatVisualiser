#pragma once

#include <vector>
#include <memory>
#include <functional>

#include "glm/vec2.hpp"

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

		// Struct to be overidden during a graph
		// Nodes can also be in an imaginary space
		//	E.g - Grids can work with coordinates
		//	    - World space
		struct Node 
		{
			Node()			= default;
			virtual ~Node() = default;

			Node(const Node& other)					= delete;
			Node(Node&& other) noexcept				= delete;
			Node& operator=(const Node& other)		= delete;
			Node& operator=(Node&& other) noexcept	= delete;

			virtual std::unique_ptr<Node> Clone() const = 0;
			virtual float GetX() const = 0;
			virtual float GetY() const = 0;
		};

		struct Connection final
		{
			Node* fromNodePtr{};
			Node* toNodePtr{};
			float weight{};

			bool operator==(const Connection& other) const
			{
				return fromNodePtr == other.fromNodePtr
					&& toNodePtr == other.toNodePtr
					&& abs(weight - other.weight) < FLT_EPSILON;
			}
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

			std::vector<Connection> GetConnectionsFromNode(size_t idx) const noexcept;
			std::vector<Connection> GetConnectionsFromNode(Node* nodePtr) const noexcept;

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

			// Fills up the path
			// Returns whether it finds a path possible
			bool FindPath(size_t startIdx, size_t goalIdx, size_t maxSteps = 100);
			bool FindPath(Node* startNodePtr, Node* goalNodePtr, size_t maxSteps = 100);
			const std::vector<Node*>& GetPath() const;
			Graph& GetGraph(); // Returns a reference to adjust the graph in the future

		private:
			struct NodeRecord final
			{
				Node* nodePtr{};
				Connection connection{};
				float costSoFar = 0.f; // accumulated g-costs of all the connections leading up to this one
				float estimatedTotalCost = 0.f; // f-cost (= costSoFar + h-cost)

				bool operator==(const NodeRecord& other) const
				{
					return nodePtr == other.nodePtr
						&& connection == other.connection
						&& abs(costSoFar - other.costSoFar) < FLT_EPSILON
						&& abs(estimatedTotalCost - other.estimatedTotalCost) < FLT_EPSILON;
				}

				bool operator<(const NodeRecord& other) const
				{
					return estimatedTotalCost < other.estimatedTotalCost;
				}
			};

			float GetHeuristicCost(Node* fromNodePtr, Node* toNodePtr) const;

			Graph m_Graph{};
			const Heuristic m_HeuristicFunc{};
			std::vector<Node*> m_Path{};
		};
	}
}
