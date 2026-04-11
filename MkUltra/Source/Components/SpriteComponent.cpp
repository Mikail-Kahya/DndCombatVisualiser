#include "SpriteComponent.h"

#include <windows.h>

#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"

using namespace mk;

SpriteComponent::SpriteComponent(Texture2D* texturePtr)
{
	if (texturePtr != nullptr)
		SetTexture(texturePtr, true);
}

SpriteComponent::SpriteComponent(const std::string& file)
{
	SetTexture(ResourceManager::GetInstance().LoadTexture(file), true);
}


void SpriteComponent::Render() const
{
	if (!m_TexturePtr)
		return;

	Renderer::GetInstance().RenderTexture(	*m_TexturePtr, static_cast<int>(m_Width), static_cast<int>(m_Height),
											GetRenderPosition(), m_Anchor, GetOwner()->GetRotation(),
											m_SrcPos, static_cast<int>(m_SrcSize.x), static_cast<int>(m_SrcSize.y),
											m_FlipAxis.first, m_FlipAxis.second);
}

void SpriteComponent::LateUpdate()
{
	const float newDepth{ GetOwner()->GetWorldDepth() };
	
	if (abs(m_Depth - newDepth) > FLT_EPSILON)
	{
		m_Depth = newDepth;
		Renderer::GetInstance().FlagDepthDirty();
	}
}

Texture2D* SpriteComponent::GetTexture() const
{
	return m_TexturePtr;
}

Vector2 SpriteComponent::GetRenderPosition() const
{
	Vector2 position{ GetOwner()->GetWorldPosition() };
	position.x -= m_Anchor.x * m_Width;
	position.y -= m_Anchor.y * m_Height;
	return { position.x, position.y };
}

float SpriteComponent::GetRenderDepth() const
{
	return m_Depth;
}

std::pair<bool, bool> SpriteComponent::IsFlipped() const
{
	return m_FlipAxis;
}

const Vector2& SpriteComponent::GetAnchor() const
{
	return m_Anchor;
}

float SpriteComponent::GetWidth() const
{
	return m_Width;
}

float SpriteComponent::GetHeight() const
{
	return m_Height;
}

const Vector2& SpriteComponent::GetSrcPosition() const
{
	return m_SrcPos;
}

const Vector2& SpriteComponent::GetSrcSize() const
{
	return m_SrcSize;
}


void SpriteComponent::SetTexture(Texture2D* texturePtr, bool useTextureSize)
{
	m_TexturePtr = texturePtr;
	m_SrcPos = {};
	m_SrcSize = { m_TexturePtr->GetSize() };

	if (useTextureSize)
	{
		const Vector2 size{ texturePtr->GetSize() };
		SetWidth(size.x);
		SetHeight(size.y);
	}
}

void SpriteComponent::SetAnchor(const Vector2& anchor)
{
	m_Anchor = anchor;
}

void SpriteComponent::SetFlipHorizontal(bool isFlipped)
{
	m_FlipAxis.first = isFlipped;
}

void SpriteComponent::SetFlipVertical(bool isFlipped)
{
	m_FlipAxis.second = isFlipped;
}

void SpriteComponent::SetWidth(float width)
{
	m_Width = width;
}

void SpriteComponent::SetHeight(float height)
{
	m_Height = height;
}

void SpriteComponent::SetSrcPosition(const Vector2& position)
{
	m_SrcPos = position;
}

void SpriteComponent::SetSrcSize(const Vector2& dimensions)
{
	m_SrcSize = dimensions;
}
