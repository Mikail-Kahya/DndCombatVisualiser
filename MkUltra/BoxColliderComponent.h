#pragma once
#include <set>

#include "MkMath.h"
#include "IComponent.h"
#include "IObserver.h"

namespace mk
{
	class BoxColliderComponent;

	enum class CollisionType : uint8_t
	{
		block,
		overlap,
		ignore
	};

	struct CollisionSettings
	{
		CollisionType collisionStatic{ CollisionType::block };
		CollisionType collisionDynamic{ CollisionType::overlap };
	};

	struct CollisionInfo
	{
		//Vector2 intersectionPoint{};
		Vector2 impactNormal{};
		Vector2 preCollisionPos{};
		Vector2 postCollisionPos{};
		Vector2 velocity{};
		BoxColliderComponent* hitCompPtr{ nullptr };
		//float distance{};
		float entryTime{};
		float remainingTime{};
	};

	struct OverlapEvent : public IEvent
	{
		OverlapEvent()				= default;
		~OverlapEvent() override	= default;
		OverlapEvent(const OverlapEvent& other)					= delete;
		OverlapEvent(OverlapEvent&& other) noexcept				= delete;
		OverlapEvent& operator=(const OverlapEvent& other)		= delete;
		OverlapEvent& operator=(OverlapEvent&& other) noexcept	= delete;

		CollisionInfo info{};
	};

	struct BlockEvent : public IEvent
	{
		BlockEvent()			= default;
		~BlockEvent() override	= default;
		BlockEvent(const BlockEvent& other)					= delete;
		BlockEvent(BlockEvent&& other) noexcept				= delete;
		BlockEvent& operator=(const BlockEvent& other)		= delete;
		BlockEvent& operator=(BlockEvent&& other) noexcept	= delete;

		CollisionInfo info{};
	};

	class BoxColliderComponent final : public IComponent, public ISubject
	{
	public:
		BoxColliderComponent() = default;
		~BoxColliderComponent() override;

		BoxColliderComponent(const BoxColliderComponent& other)					= delete;
		BoxColliderComponent(BoxColliderComponent&& other) noexcept				= delete;
		BoxColliderComponent& operator=(const BoxColliderComponent& other)		= delete;
		BoxColliderComponent& operator=(BoxColliderComponent&& other) noexcept	= delete;

		void Start() override;

		void Collide(const CollisionInfo& info);
		bool IsIgnoring(GameObject* objectPtr) const noexcept;
		void Ignore(GameObject* colliderPtr, bool mutuallyIgnore = true) noexcept;
		void StopIgnoring(GameObject* colliderPtr, bool mutuallyStopIgnore = true) noexcept;

		CollisionSettings GetCollision() const noexcept;
		const Vector2& GetBoxExtent() const noexcept;

		void SetCollision(CollisionSettings settings) noexcept;
		void SetExtent(const Vector2& extent) noexcept;

	private:
		void HandleOverlap(const CollisionInfo& info);
		void HandleBlock(const CollisionInfo& info);

		std::set<GameObject*> m_IgnoreObjects{};
		CollisionSettings m_CollisionSettings{};
		Vector2 m_Extent{30.f, 30.f};
	};
}
