#pragma once

#include <vector>
#include <memory>
#include <functional>

namespace mk
{
	namespace algo
	{
		/* 
		======================================
		ASTAR PATHFINDING
		====================================== 
		*/

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

		
	}
}