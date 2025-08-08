#include "PlayerCommand.h"

#include "GameObject.h"
#include "Components/Character/MovementComponent.h"

using namespace mk;

MoveCommand::MoveCommand(GameObject* gameObject, const glm::vec2& direction)
	: GameObjectCommand(gameObject)
	, m_Direction{ direction }
{
	m_MoveCompPtr = gameObject->AddComponent<MovementComponent>();
}

void MoveCommand::Execute()
{
}
