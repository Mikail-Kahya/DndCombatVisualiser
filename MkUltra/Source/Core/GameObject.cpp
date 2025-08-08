#include <string>
#include <algorithm>

#include "GameObject.h"
#include "IComponent.h"

using namespace mk;

GameObject::GameObject(const std::string& name)
	: m_Name{ name }
	, m_Components{}
	, m_ComponentBuffer{}
{
}

GameObject::~GameObject() = default;

GameObject::GameObject(GameObject&& other) noexcept
	: m_Name{ std::move(other.m_Name) }
	, m_Destroy{ other.m_Destroy }
	, m_LocalTransform{ other.m_LocalTransform }
	, m_WorldTransform{ other.m_WorldTransform }
	, m_PositionIsDirty{ other.m_PositionIsDirty }
	, m_Parent{ other.m_Parent }
	, m_Children{ std::move(other.m_Children) }
	, m_Components{ std::move(other.m_Components) }
	, m_ComponentBuffer{ std::move(other.m_ComponentBuffer) }
	
{
	other.m_Parent = nullptr;
}

GameObject& GameObject::operator=(GameObject&& other) noexcept
{
	m_Name = std::move(other.m_Name);
	m_Destroy = other.m_Destroy;
	m_LocalTransform = std::move(other.m_LocalTransform);
	m_WorldTransform = std::move(other.m_WorldTransform);
	m_PositionIsDirty = other.m_PositionIsDirty;
	m_Parent = other.m_Parent;
	m_Children = std::move(other.m_Children);
	m_Components = std::move(other.m_Components);
	m_ComponentBuffer = std::move(other.m_ComponentBuffer);

	other.m_Parent = nullptr;

	return *this;
}

void GameObject::RemoveComponent(const std::unique_ptr<IComponent>& component)
{
	component->Destroy();
}

void GameObject::Update()
{
	for (const auto& component : m_Components)
		component->Update();
}

void GameObject::FixedUpdate()
{
	for (const auto& component : m_Components)
		component->FixedUpdate();
}

void GameObject::LateUpdate()
{
	for (const auto& component : m_Components)
		component->LateUpdate();

	ComponentCleanup();
}

void GameObject::ComponentCleanup()
{
	// Remove destroy flagged components
	auto eraseIt = std::stable_partition(m_Components.begin(), m_Components.end(), [](const std::unique_ptr<IComponent>& component)
		{
			return !component->DestroyFlagged();
		});

	m_Components.erase(eraseIt, m_Components.end());

	// Move components and flush buffer
	m_Components.insert(m_Components.end(),
		std::make_move_iterator(m_ComponentBuffer.begin()),
		std::make_move_iterator(m_ComponentBuffer.end()));
	m_ComponentBuffer.clear();
}

void GameObject::AddChild(GameObject* childPtr)
{
	m_Children.push_back(childPtr);
}

void GameObject::RemoveChild(GameObject* childPtr)
{
	m_Children.erase(std::ranges::find(m_Children, childPtr));
}

bool GameObject::IsChild(GameObject* childPtr) const
{
	const auto foundChild{ std::ranges::find(m_Children, childPtr) };
	return foundChild != m_Children.cend();
}

void GameObject::Destroy()
{
	m_Destroy = true;
	for (GameObject* childPtr : m_Children)
		childPtr->Destroy();
}

void GameObject::ClearDestroy()
{
	m_Destroy = false;
	for (GameObject* childPtr : m_Children)
		childPtr->ClearDestroy();
}

bool GameObject::DestroyFlagged() const
{
	return m_Destroy;
}

const std::string& GameObject::GetName() const
{
	return m_Name;
}

glm::vec2 GameObject::GetWorldPosition()
{
	if (m_PositionIsDirty)
		UpdateWorldPosition();
	return m_WorldTransform.GetPosition();
}

glm::vec2 GameObject::GetLocalPosition() const
{
	return m_LocalTransform.GetPosition();
}

float GameObject::GetWorldDepth()
{
	if (m_PositionIsDirty)
		UpdateWorldPosition();
	return m_WorldTransform.GetDepth();
}

float GameObject::GetLocalDepth() const
{
	return m_LocalTransform.GetDepth();
}

float GameObject::GetRotation()
{
	if (m_RotationIsDirty)
		UpdateWorldRotation();
	return m_WorldTransform.GetRotation();
}

glm::vec2 GameObject::GetForward()
{
	const float angleRad{ GetRotation() * 3.14f / 180.f };
	return { cosf(angleRad), sinf(angleRad) };
}

bool GameObject::IsStatic() const
{
	return m_IsStatic;
}

void GameObject::SetLocalPosition(const glm::vec2& position)
{
	m_LocalTransform.SetPosition(position);
	FlagPositionDirty();
}

void GameObject::AddLocalOffset(const glm::vec2& offset)
{
	m_LocalTransform.AddOffset(offset);
	FlagPositionDirty();
}

void GameObject::SetLocalDepth(float depth)
{
	m_LocalTransform.SetDepth(depth);
	FlagPositionDirty();
}

void GameObject::AddLocalDepth(float deltaDepth)
{
	m_LocalTransform.AddDepth(deltaDepth);
	FlagPositionDirty();
}

void GameObject::SetRotation(float rotation)
{
	m_LocalTransform.SetRotation(rotation);
	FlagRotationDirty();
}

void GameObject::AddRotation(float deltaRotation)
{
	m_LocalTransform.AddRotation(deltaRotation);
	FlagRotationDirty();
}

void GameObject::SetStatic(bool isStatic)
{
	m_IsStatic = isStatic;
}

void GameObject::UpdateWorldPosition()
{
	if (!m_PositionIsDirty)
		return;

	if (m_Parent == nullptr)
	{
		m_WorldTransform.SetPosition(m_LocalTransform.GetPosition());
		m_WorldTransform.SetDepth(m_LocalTransform.GetDepth());
	}
	else
	{
		m_WorldTransform.SetPosition(m_Parent->GetWorldPosition() + m_LocalTransform.GetPosition());
		m_WorldTransform.AddDepth(m_Parent->GetWorldDepth() + m_LocalTransform.GetDepth());
	}

	m_PositionIsDirty = false;
}

void GameObject::UpdateWorldRotation()
{
	if (!m_RotationIsDirty)
		return;

	if (m_Parent == nullptr)
		m_WorldTransform.SetRotation(m_LocalTransform.GetRotation());
	else
		m_WorldTransform.SetRotation(m_Parent->GetRotation() + m_LocalTransform.GetRotation());

	m_RotationIsDirty = false;
}

void GameObject::FlagPositionDirty()
{
	m_PositionIsDirty = true;
	for (GameObject* childPtr : m_Children)
		childPtr->FlagPositionDirty();
}

void GameObject::FlagRotationDirty()
{
	m_RotationIsDirty = true;
	for (GameObject* childPtr : m_Children)
		childPtr->FlagRotationDirty();
}

void GameObject::SetParent(GameObject* parentPtr, bool keepWorldPosition)
{
	if (m_Parent == parentPtr || this == parentPtr || IsChild(parentPtr))
		return;

	if (parentPtr == nullptr)
	{
		SetLocalPosition(GetWorldPosition());
		SetLocalDepth(GetWorldDepth());
	}
	else
	{
		if (keepWorldPosition)
		{
			SetLocalPosition(m_WorldTransform.GetPosition() - parentPtr->GetWorldPosition());
			SetLocalDepth(m_WorldTransform.GetDepth() - parentPtr->GetWorldDepth());
		}
		FlagPositionDirty();
	}

	if (m_Parent != nullptr)
		m_Parent->RemoveChild(this);
	m_Parent = parentPtr;
	if (m_Parent != nullptr)
		m_Parent->AddChild(this);
}

GameObject* GameObject::GetParent() const
{
	return m_Parent;
}

int GameObject::GetChildCount() const
{
	return static_cast<int>(m_Children.size());
}

GameObject* GameObject::GetChildAt(int index) const
{
	if (index > GetChildCount())
		return nullptr;
	return m_Children[index];
}

GameObject* GameObject::GetChildWithName(const std::string& name, bool recursively) const
{
	GameObject* foundChildPtr = *std::find_if(m_Children.begin(), m_Children.end(), [&name](GameObject* childPtr)
		{
			return childPtr->GetName() == name;
		});

	if (foundChildPtr != nullptr)
		return foundChildPtr;

	if (!recursively)
		return nullptr;

	for (const GameObject* childPtr : m_Children)
	{
		foundChildPtr = childPtr->GetChildWithName(name, recursively);
		if (foundChildPtr != nullptr )
			return foundChildPtr;
	}
		

	return nullptr;
}
