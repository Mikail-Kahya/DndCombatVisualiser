#include "IRenderer.h"

#include "Renderer.h"

mk::IRenderer::IRenderer()
{
	Renderer::GetInstance().RegisterRenderer(this);
}

mk::IRenderer::~IRenderer()
{
	Renderer::GetInstance().DeregisterRenderer(this);
}

void mk::IRenderer::SetDepth(float newDepth)
{
	m_Depth = newDepth;
}

void mk::IRenderer::SetVisibility(bool isVisible)
{
	m_IsVisible = isVisible;
}

float mk::IRenderer::GetDepth() const
{
	return m_Depth;
}

bool mk::IRenderer::IsVisible() const
{
	return m_IsVisible;
}
