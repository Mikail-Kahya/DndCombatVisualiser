#include "SceneManager.h"

using namespace mk;

void SceneManager::FixedUpdate()
{
	m_Scene->FixedUpdate();
}

void SceneManager::Update()
{
	m_Scene->Update();
}

void SceneManager::LateUpdate()
{
	m_Scene->LateUpdate();
}

TimeManager& SceneManager::GetTimeManager()
{
	return m_TimeManager;
}

const TimeManager& mk::Time()
{
	return SceneManager::GetInstance().GetTimeManager();
}

void SceneManager::LoadScene(const std::string& name, std::function<void(Scene&)> sceneLoader)
{
	m_LoadedScene = std::make_unique<Scene>(name);
	sceneLoader(*m_LoadedScene);
	m_Scene = std::move(m_LoadedScene);
}

Scene& SceneManager::GetScene() const
{
	return *m_Scene;
}
