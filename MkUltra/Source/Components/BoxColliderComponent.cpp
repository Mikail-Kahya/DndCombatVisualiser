#include "BoxColliderComponent.h"

#include <iostream>

#include "GameObject.h"
#include "Geometry.h"
#include "PhysicsSystem.h"

using namespace mk;

BoxColliderComponent::~BoxColliderComponent()
{
	PhysicsSystem::GetInstance().UnRegisterCollider(this);
}

void BoxColliderComponent::Start()
{
	IComponent::Start();
	PhysicsSystem::GetInstance().RegisterCollider(this);
}

void BoxColliderComponent::Collide(const CollisionInfo& info)
{
	CollisionType type = info.hitCompPtr->GetOwner()->IsStatic() ? m_CollisionSettings.collisionStatic : m_CollisionSettings.collisionDynamic;
	
	switch (type)
	{
	case CollisionType::block:
		HandleBlock(info);
		break;
	case CollisionType::overlap:
		if (!GetOwner()->IsStatic())
			HandleOverlap(info);
		break;
	}
}

bool BoxColliderComponent::IsIgnoring(GameObject* objectPtr) const noexcept
{
	return m_IgnoreObjects.contains(objectPtr);
}

void BoxColliderComponent::Ignore(GameObject* colliderPtr, bool mutuallyIgnore) noexcept
{
	m_IgnoreObjects.insert(colliderPtr);
	if (mutuallyIgnore)
	{
		if (BoxColliderComponent* otherColliderPtr = colliderPtr->GetComponent<BoxColliderComponent>())
			otherColliderPtr->Ignore(GetOwner(), false);
	}
}

void BoxColliderComponent::StopIgnoring(GameObject* colliderPtr, bool mutuallyStopIgnore) noexcept
{
	if (!m_IgnoreObjects.contains(colliderPtr))
		return;

	if (mutuallyStopIgnore)
	{
		if (BoxColliderComponent* otherColliderPtr = colliderPtr->GetComponent<BoxColliderComponent>())
			otherColliderPtr->StopIgnoring(GetOwner(), false);
	}
	m_IgnoreObjects.erase(colliderPtr);
}

CollisionSettings BoxColliderComponent::GetCollision() const noexcept
{
	return m_CollisionSettings;
}

const glm::vec2& BoxColliderComponent::GetBoxExtent() const noexcept
{
	return m_Extent;
}

void BoxColliderComponent::SetCollision(CollisionSettings settings) noexcept
{
	auto clampCollision = [](CollisionType type) -> CollisionType
		{
			constexpr int lastIdx{ static_cast<int>(CollisionType::ignore) };
			int idx{ static_cast<int>(type) };
			if (idx < 0 || idx > lastIdx)
				idx = lastIdx;
			return static_cast<CollisionType>(idx);
		};

	settings.collisionDynamic = clampCollision(settings.collisionDynamic);
	settings.collisionStatic = clampCollision(settings.collisionStatic);
	m_CollisionSettings = settings;
}

void BoxColliderComponent::SetExtent(const glm::vec2& extent) noexcept
{
	m_Extent = extent;
}

void BoxColliderComponent::HandleOverlap(const CollisionInfo& info)
{
	auto overlapEvent{ std::make_unique<OverlapEvent>() };
	overlapEvent->info = info;
	Notify(std::move(overlapEvent));
}

void BoxColliderComponent::HandleBlock(const CollisionInfo& info)
{
	// push 
	float magnitude = glm::length(info.velocity) * info.remainingTime;
	float dot = glm::dot(info.velocity, info.impactNormal);

	if (dot > 0.0f) 
		dot = 1.0f;
	else if (dot < 0.0f) 
		dot = -1.0f;

	glm::vec2 velocity{ dot * info.impactNormal * magnitude };

	GetOwner()->SetLocalPosition(info.preCollisionPos + velocity * info.entryTime);

	auto blockEvent{ std::make_unique<BlockEvent>() };
	blockEvent->info = info;
	Notify(std::move(blockEvent));
}