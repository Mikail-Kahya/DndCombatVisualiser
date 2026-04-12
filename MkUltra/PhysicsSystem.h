#pragma once
#include <vector>

#include "BoxColliderComponent.h"
#include "ISingleton.h"

namespace mk
{
	struct PhysicsInfo
	{
		Vector2 position{};
		Vector2 boxExtent{};
		Vector2 prevPos{};
	};

	struct BoxBounds
	{
		Vector2 min{};
		Vector2 max{};
	};

	class PhysicsSystem final : public ISingleton<PhysicsSystem>
	{
		friend class ISingleton<PhysicsSystem>;
	public:
		~PhysicsSystem() override = default;

		PhysicsSystem(const PhysicsSystem& other)					= delete;
		PhysicsSystem(PhysicsSystem&& other) noexcept				= delete;
		PhysicsSystem& operator=(const PhysicsSystem& other)		= delete;
		PhysicsSystem& operator=(PhysicsSystem&& other)	noexcept	= delete;

#ifndef NDEBUG
		void DrawDebug() const;
#endif

		void Update();

		void RegisterCollider(BoxColliderComponent* colliderPtr);
		void UnRegisterCollider(BoxColliderComponent* colliderPtr);

	private:
		using Collider = std::pair<BoxColliderComponent*, PhysicsInfo>;

		PhysicsSystem() = default;

		void HandleCollision() const;
		void UpdateInformation();
		void UpdatePrevPositions();

		static PhysicsInfo GetPhysicsInfo(const BoxColliderComponent* colliderPtr);
		static bool IsOverlapping(const PhysicsInfo& a, const PhysicsInfo& b);
		static CollisionInfo SweepCollider(const Collider& a, const Collider& b);
		static CollisionInfo ResolveCollider(const Collider& a, const Collider& b);
		static void GetVertices(const Vector2& position, const Vector2& boxExtent, std::vector<Vector2>& vertices);
		static BoxBounds GetMinMax(const PhysicsInfo& info);

		std::vector<Collider> m_PhysicsBuffer{};
	};
}
