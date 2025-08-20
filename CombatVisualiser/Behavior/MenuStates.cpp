#include "MenuStates.h"

#include "GameObject.h"
#include "SpriteComponent.h"
#include "ResourceManager.h"

using namespace mk;

StartState::StartState(GameObject* startButton)
	: m_RenderCompPtr{ startButton->GetComponent<SpriteComponent>() }
{
}

void StartState::OnEnter()
{
	GameObjectState::OnEnter();
	m_RenderCompPtr->SetTexture(ResourceManager::GetInstance().LoadTexture("ButtonSelected.png"), false);
}

void StartState::OnExit()
{
	GameObjectState::OnExit();
	m_RenderCompPtr->SetTexture(ResourceManager::GetInstance().LoadTexture("Button.png"), false);
}

std::string StartState::TransitionState()
{
	return "";
}

VersusState::VersusState(GameObject* versusButton)
	: m_RenderCompPtr{ versusButton->GetComponent<SpriteComponent>() }
{
}

void VersusState::OnEnter()
{
	GameObjectState::OnEnter();
	m_RenderCompPtr->SetTexture(ResourceManager::GetInstance().LoadTexture("Button.png"), false);
}

void VersusState::OnExit()
{
	GameObjectState::OnExit();
	m_RenderCompPtr->SetTexture(ResourceManager::GetInstance().LoadTexture("Button.png"), false);
}

std::string VersusState::TransitionState()
{
	return "";
}
