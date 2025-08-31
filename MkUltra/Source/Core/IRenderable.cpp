#include "IRenderable.h"

#include "Renderer.h"

mk::IRenderable::IRenderable()
{
	Renderer::GetInstance().RegisterRenderer(this);
}

mk::IRenderable::~IRenderable()
{
	Renderer::GetInstance().DeregisterRenderer(this);
}

void mk::IRenderable::SetDepth(float newDepth)
{
	m_Depth = newDepth;
}

void mk::IRenderable::SetVisibility(bool isVisible)
{
	m_IsVisible = isVisible;
}

float mk::IRenderable::GetDepth() const
{
	return m_Depth;
}

bool mk::IRenderable::IsVisible() const
{
	return m_IsVisible;
}
