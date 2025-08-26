#include "Pathfinding.h"

using namespace mk;
using namespace algo;

Graph::Graph(const Graph& other)
	: connections{ other.connections }
{
	for (const auto& node : other.nodes)
		nodes.emplace_back(node->Clone());
}

Graph& Graph::operator=(const Graph& other)
{
	connections = other.connections;
	for (const auto& node : other.nodes)
		nodes.emplace_back(node->Clone());

	return *this;
}

Connection* Graph::AddConnection(size_t fromIdx, size_t toIdx, float weight) noexcept
{
	if (nodes.size() < fromIdx || nodes.size() < toIdx)
		return nullptr;

	connections.emplace_back(Connection{ nodes[fromIdx].get(), nodes[toIdx].get(), weight });
	return &connections.back();
}

bool Graph::RemoveConnection(size_t fromIdx, size_t toIdx) noexcept
{
	if (nodes.size() < fromIdx || nodes.size() < toIdx)
		return false;

	const Node* fromNodePtr{ nodes[fromIdx].get() };
	const Node* toNodePtr{ nodes[toIdx].get() };

	const size_t nrConnections = 
		std::erase_if(	connections, 
						[fromNodePtr, toNodePtr](const Connection& connection)
						{
							return connection.fromNodePtr == fromNodePtr && connection.toNodePtr == toNodePtr;
						});
	return nrConnections;
}

