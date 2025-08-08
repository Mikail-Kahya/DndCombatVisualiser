#pragma once
#include "IComponent.h"
#include "glm/vec2.hpp"

namespace mk
{
	/*	The movement component stores the maximum movement someone is allowed based on their race and additional feats
	 *	
	 *	The base movement is determined by the race
	 *	The additional movement needs to be inserted into the character sheet
	 *	A* will be used to generate the best pathing on the map
	 */

	class MovementComponent final : public IComponent
	{
	public:
		MovementComponent();
		~MovementComponent() override = default;

		MovementComponent(const MovementComponent& other)				= delete;
		MovementComponent(MovementComponent&& other)					= delete;
		MovementComponent& operator=(const MovementComponent& other)	= delete;
		MovementComponent& operator=(MovementComponent&& other)			= delete;

		void Update() override;

	private:
	};
}
