#include "PhysicsSystem.h"

#include <algorithm>

#include "BoxColliderComponent.h"
#include "GameObject.h"
#include "Geometry.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "glm/gtx/intersect.hpp"

using namespace mk;

#ifndef NDEBUG
void PhysicsSystem::DrawDebug() const
{
	for (const Collider& collider : m_PhysicsBuffer)
	{
		const PhysicsInfo& info{ collider.second };
		const int halfWidth{ static_cast<int>(info.boxExtent.x) };
		const int halfHeight{ static_cast<int>(info.boxExtent.y) };
		Renderer::GetInstance().RenderRect(
			static_cast<int>(info.position.x) - halfWidth,
			static_cast<int>(info.position.y) - halfHeight,
			halfWidth * 2,
			halfHeight * 2,
			Color{ 155, 155, 0, 1 }
		);
	}
}
#endif

void PhysicsSystem::Update()
{
	if (m_PhysicsBuffer.empty())
		return;

	UpdateInformation();
	HandleCollision();
	UpdatePrevPositions();
}

void PhysicsSystem::RegisterCollider(BoxColliderComponent* colliderPtr)
{
	// Avoid duplicates
	const auto foundIter = std::find_if(m_PhysicsBuffer.begin(), m_PhysicsBuffer.end(), [colliderPtr](const Collider& collider)
		{
			return colliderPtr == collider.first;
		});

	if (foundIter != m_PhysicsBuffer.end())
		return;

	PhysicsInfo info{ GetPhysicsInfo(colliderPtr) };
	info.prevPos = colliderPtr->GetOwner()->GetWorldPosition();
	m_PhysicsBuffer.emplace_back(Collider{ colliderPtr, info });
}

void PhysicsSystem::UnRegisterCollider(BoxColliderComponent* colliderPtr)
{
	const auto foundIter = std::find_if(m_PhysicsBuffer.begin(), m_PhysicsBuffer.end(), [colliderPtr](const Collider& collider)
		{
			return colliderPtr == collider.first;
		});
	if (foundIter != m_PhysicsBuffer.end())
		m_PhysicsBuffer.erase(foundIter);
}

void PhysicsSystem::HandleCollision() const
{
	const size_t nrColliders{ m_PhysicsBuffer.size() };
	// iterate over every one except the last
	// the last iteration was covered by every iteration before it
	for (size_t idxFirst{}; idxFirst < nrColliders - 1; ++idxFirst)
	{
		for (size_t idxSecond{ idxFirst + 1 }; idxSecond < nrColliders; ++idxSecond)
		{
			const Collider& firstCollider{ m_PhysicsBuffer[idxFirst] };

			const Collider& secondCollider{ m_PhysicsBuffer[idxSecond] };
			const bool isFirstIgnoring{ firstCollider.first->IsIgnoring(secondCollider.first->GetOwner()) };
			const bool isSecondIgnoring{ secondCollider.first->IsIgnoring(firstCollider.first->GetOwner()) };

			if (isFirstIgnoring && isSecondIgnoring)
				continue;

			if (!IsOverlapping(firstCollider.second, secondCollider.second))
				continue;

			if (!isFirstIgnoring && !firstCollider.first->GetOwner()->IsStatic())
				firstCollider.first->Collide(SweepCollider(firstCollider, secondCollider));

			if (!isSecondIgnoring && !secondCollider.first->GetOwner()->IsStatic())
				secondCollider.first->Collide(SweepCollider(secondCollider, firstCollider));
		}
	}
}

void PhysicsSystem::UpdateInformation()
{
	for (Collider& collider : m_PhysicsBuffer)
	{
		const glm::vec2 prevPos{ collider.second.prevPos };
		collider.second = GetPhysicsInfo(collider.first);
		collider.second.prevPos = prevPos;
	}
}

void PhysicsSystem::UpdatePrevPositions()
{
	for (Collider& collider : m_PhysicsBuffer)
		collider.second.prevPos = collider.first->GetOwner()->GetWorldPosition();
}

PhysicsInfo PhysicsSystem::GetPhysicsInfo(const BoxColliderComponent* colliderPtr)
{
	return {
		.position = colliderPtr->GetOwner()->GetWorldPosition(),
		.boxExtent = colliderPtr->GetBoxExtent()
	};
}

bool PhysicsSystem::IsOverlapping(const PhysicsInfo& a, const PhysicsInfo& b)
{
	const auto box{ Geometry::GetBoxMinMax(a.position, a.boxExtent) };
	const auto otherBox{ Geometry::GetBoxMinMax(b.position, b.boxExtent) };

	if (box.topRight.x < otherBox.bottomLeft.x ||	// left check
		otherBox.topRight.x < box.bottomLeft.x ||	// right check
		otherBox.topRight.y < box.bottomLeft.y ||	// top check
		box.topRight.y < otherBox.bottomLeft.y)		// bottom check
		return false;
	return true;
}

CollisionInfo PhysicsSystem::SweepCollider(const Collider& a, const Collider& b)
{
	const glm::vec2 displacement{ a.second.position - a.second.prevPos };

	CollisionInfo result{};
	//result.distance = FLT_MAX;
	result.preCollisionPos = a.second.prevPos;
	result.postCollisionPos = a.second.position;
	result.hitCompPtr = b.first;
	result.velocity = displacement / Time().deltaTime;

	// https://www.gamedev.net/tutorials/programming/general-and-gameplay-programming/swept-aabb-collision-detection-and-response-r3084/

	// find the distance between the objects on the near and far sides
	glm::vec2 invEntry{};
	glm::vec2 invExit{};
	
	const auto minMax{ GetMinMax(a.second) };
	const auto otherMinMax{ GetMinMax(b.second) };
	
	if (result.velocity.x > 0.0f)
	{
		invEntry.x = otherMinMax.min.x - minMax.max.x;
		invExit.x = otherMinMax.max.x - minMax.min.x;
	}
	else
	{
		invEntry.x = otherMinMax.max.x - minMax.min.x;
		invExit.x = otherMinMax.min.x - minMax.max.x;
	}
	
	if (result.velocity.y > 0.0f)
	{
		invEntry.y = otherMinMax.min.y - minMax.max.y;
		invExit.y = otherMinMax.max.y - minMax.min.y;
	}
	else
	{
		invEntry.y = otherMinMax.max.y - minMax.min.y;
		invExit.y = otherMinMax.min.y - minMax.max.y;
	}
	
	
	// find time of collision and time of leaving for each axis (if statement is to prevent divide by zero)
	glm::vec2 entry{ -std::numeric_limits<float>::infinity() };
	glm::vec2 exit{ std::numeric_limits<float>::infinity() };
	
	
	if (result.velocity.x != 0.0f)
	{
		entry.x = invEntry.x / result.velocity.x;
		exit.x = invExit.x / result.velocity.x;
	}
	
	if (result.velocity.y != 0.0f)
	{
		entry.y = invEntry.y / result.velocity.y;
		exit.y = invExit.y / result.velocity.y;
	}
	
	// calculate normal of collided surface
	if (entry.x > entry.y)
		result.impactNormal = (invEntry.x < 0.0f) ? glm::vec2{ 1, 0 } : glm::vec2{ -1, 0 };
	else
		result.impactNormal = (invEntry.y < 0.0f) ? glm::vec2{ 0, 1 } : glm::vec2{ 0, -1 };

	// find the earliest/latest times of collisionfloat 
	const float entryTime = std::max(entry.x, entry.y);
	result.entryTime = entryTime;
	result.remainingTime = Time().deltaTime - result.entryTime;

	return result;
}

CollisionInfo PhysicsSystem::ResolveCollider(const Collider& a, const Collider& b)
{
	CollisionInfo result{};
	//result.distance = FLT_MAX;
	result.preCollisionPos = a.second.position;
	result.hitCompPtr = b.first;
	result.velocity = {};

	// Math help https://imois.in/posts/line-intersections-with-cross-products/
	std::vector<glm::vec2> vertices{};
	GetVertices(b.second.position, b.second.boxExtent, vertices);

	glm::vec2 finalIntersection{};
	const glm::vec2 ray{ b.second.position - a.second.position };
	const glm::vec3 pos{ a.second.position, 1 };
	const glm::vec3 end{ b.second.position, 1 };
	constexpr int nrVertices{ 4 };
	for (int idx{}; idx < nrVertices; ++idx)
	{
		const glm::vec2& p1{ vertices[idx] };
		const glm::vec2& p2{ vertices[(idx + 1) % nrVertices] };
		const glm::vec2 edge{ p2 - p1 };
	
		// place lines in projection space
		const glm::vec3 line1{ glm::cross(glm::vec3{p1, 1}, glm::vec3{p2,1}) };
		const glm::vec3 line2{ glm::cross(pos, end) };
		const glm::vec3 solution{ glm::cross(line1, line2) };
		const glm::vec2 intersection{ solution.x / solution.z, solution.y / solution.z };
		if (solution.z < FLT_EPSILON)
			continue;
	
		// check if point isn't to out of vertices on the line
		const glm::vec2 intersectToP1{ p1 - intersection };
		if (glm::cross(glm::vec3{ ray, 0 }, glm::vec3{ intersectToP1, 0 }).z < 0) // < 0 is to the left of P1
			continue;
	
		const glm::vec2 intersectToP2{ p2 - intersection };
		if (glm::cross(glm::vec3{ ray, 0 }, glm::vec3{ intersectToP2, 0 }).z > 0) // > 0 is to the right of P2
			continue;
	
		finalIntersection = intersection;
		result.impactNormal = glm::normalize(glm::cross(glm::vec3{ edge, 0 }, glm::vec3{ 0, 0, 1 }));
	}
	
	return result;
}

void PhysicsSystem::GetVertices(const glm::vec2& position, const glm::vec2& boxExtent, std::vector<glm::vec2>& vertices)
{
	constexpr int nrVertices{ 4 };
	vertices.clear();
	vertices.resize(nrVertices);
	vertices[0] = position + boxExtent;
	vertices[1] = position - glm::vec2{ boxExtent.x, -boxExtent.y };
	vertices[2] = position - boxExtent;
	vertices[3] = position - glm::vec2{ -boxExtent.x, boxExtent.y };
}

BoxBounds PhysicsSystem::GetMinMax(const PhysicsInfo& info)
{
	return { info.position - info.boxExtent, info.position + info.boxExtent };
}
