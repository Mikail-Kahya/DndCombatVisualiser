#include "Pathfinding.h"

#include <algorithm>

using namespace mk;
using namespace algo;

/* ===============================
 * GRAPH
 * ===============================*/

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
	if (nodes.size() <= fromIdx || nodes.size() <= toIdx)
		return nullptr;

	connections.emplace_back(Connection{ nodes[fromIdx].get(), nodes[toIdx].get(), weight });
	return &connections.back();
}

bool Graph::RemoveConnection(size_t fromIdx, size_t toIdx) noexcept
{
	if (nodes.size() <= fromIdx || nodes.size() <= toIdx)
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

std::vector<Connection> Graph::GetConnectionsFromNode(size_t idx) const noexcept
{
	if (nodes.size() <= idx)
		return {};
	return GetConnectionsFromNode(nodes[idx].get());
}

std::vector<Connection> Graph::GetConnectionsFromNode(Node* nodePtr) const noexcept
{
	std::vector<Connection> nodeConnections{};
	for (const Connection& connection : connections)
	{
		if (connection.fromNodePtr == nodePtr) 
			nodeConnections.emplace_back(connection);
	}
	return nodeConnections;
}

/* ===============================
 * ASTAR
 * ===============================*/

AStar::AStar(Graph&& graph, const Heuristic& heuristicFunc)
	: m_Graph{ std::move(graph) }
	, m_HeuristicFunc{ heuristicFunc }
{
}

bool AStar::FindPath(size_t startIdx, size_t goalIdx, size_t maxSteps)
{
	if (m_Graph.nodes.size() < startIdx || m_Graph.nodes.size() < goalIdx)
		return false;
	return FindPath(m_Graph.nodes[startIdx].get(), m_Graph.nodes[goalIdx].get(), maxSteps);
}

bool AStar::FindPath(Node* startNodePtr, Node* goalNodePtr, size_t maxSteps)
{
	if (!startNodePtr || !goalNodePtr)
		return false;

	bool foundPath{ false };

	std::list<NodeRecord> openList{};
	std::list<NodeRecord> closedList{};

	auto replaceNode = [](std::list<NodeRecord>& list, const NodeRecord& newRecord)
		{
			for (NodeRecord& record : list)
			{
				if (record.nodePtr == newRecord.nodePtr && record.costSoFar >= newRecord.costSoFar)
				{
					record = newRecord;
					return true;
				}
			}
			return false;
			//std::replace_if(list.begin(), list.end(), [&newRecord](const NodeRecord& record)
			//	{
			//		return record.nodePtr == newRecord.nodePtr && record.costSoFar >= newRecord.costSoFar;
			//	}, newRecord);
		};

	size_t currentStep{};

	NodeRecord currentRecord{};
	currentRecord.nodePtr = startNodePtr;
	currentRecord.estimatedTotalCost = GetHeuristicCost(startNodePtr, goalNodePtr);

	openList.push_back(currentRecord);

	// Check graph and fill closed list
	while (!openList.empty())
	{
		if (currentStep > maxSteps)
			break;

		++currentStep;

		// Get connection with the lowest F score (estimatedTotalCost)
		// std::min_element uses the < and == overloaded operator

		currentRecord = *std::min_element(openList.begin(), openList.end());
		if (currentRecord.nodePtr == goalNodePtr)
		{
			foundPath = true;
			break;
		}

		const auto& connections{ m_Graph.GetConnectionsFromNode(currentRecord.nodePtr) };
		for (const Connection& connection : connections)
		{
			NodeRecord newRecord{};
			newRecord.nodePtr = connection.toNodePtr;
			newRecord.connection = connection;
			newRecord.costSoFar = currentRecord.costSoFar + connection.weight;
			newRecord.estimatedTotalCost = newRecord.costSoFar + GetHeuristicCost(newRecord.nodePtr, goalNodePtr);

			// Remove/override any nodes found in the list and with a cheaper cost
			if (replaceNode(openList, newRecord))
				continue;
			if (replaceNode(closedList, newRecord))
				continue;

			openList.push_back(newRecord);
		}

		closedList.push_back(currentRecord);
		openList.remove(currentRecord);
	}

	if (foundPath)
	{
		// from the end to start, walk slowly backwards stepwise
		m_Path.clear();
		while (currentRecord.nodePtr != startNodePtr)
		{
			m_Path.push_back(currentRecord.nodePtr);
			Node* const currentStartNodePtr{ currentRecord.connection.fromNodePtr };

			for (const NodeRecord& record : closedList)
			{
				if (record.nodePtr != currentStartNodePtr)
					continue;

				currentRecord = record;
				break;
			}
		}

		m_Path.push_back(startNodePtr);

		// reverse path
		std::ranges::reverse(m_Path);
	}
	

	return foundPath;
}

const std::vector<Node*>& AStar::GetPath() const
{
	return m_Path;
}

Graph& AStar::GetGraph()
{
	return m_Graph;
}

float AStar::GetHeuristicCost(Node* fromNodePtr, Node* toNodePtr) const
{
	return m_HeuristicFunc(	abs(toNodePtr->GetX() - fromNodePtr->GetX()), 
							abs(toNodePtr->GetY() - fromNodePtr->GetY()));
}
