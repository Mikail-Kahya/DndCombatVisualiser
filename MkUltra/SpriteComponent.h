#pragma once

#include <string_view>

#include "IComponent.h"
#include "IRenderable.h"
#include "Transform.h"

namespace mk
{
	class Texture2D;

	class SpriteComponent : public IComponent, public IRenderable
	{
	public:
		SpriteComponent(Texture2D* texturePtr);
		SpriteComponent(const std::string& file);

		SpriteComponent(const SpriteComponent& other)				= delete;
		SpriteComponent(SpriteComponent&& other)					= delete;
		SpriteComponent& operator=(const SpriteComponent& other)	= delete;
		SpriteComponent& operator=(SpriteComponent&& other)			= delete;

		void Render() const override;
		void LateUpdate() override;

		Texture2D* GetTexture() const;
		Vector2 GetRenderPosition() const;
		float GetRenderDepth() const;
		std::pair<bool, bool> IsFlipped() const;
		const Vector2& GetAnchor() const;
		float GetWidth() const;
		float GetHeight() const;

		const Vector2& GetSrcPosition() const;
		const Vector2& GetSrcSize() const;

		void SetTexture(Texture2D* texturePtr, bool useTextureDimensions = true);
		void SetAnchor(const Vector2& anchor);
		void SetFlipHorizontal(bool isFlipped);
		void SetFlipVertical(bool isFlipped);

		void SetWidth(float width);
		void SetHeight(float height);

		void SetSrcPosition(const Vector2& position);
		void SetSrcSize(const Vector2& dimensions);

	private:
		Vector2 m_Anchor{};
		Vector2 m_SrcPos{};
		Vector2 m_SrcSize{};
		Texture2D* m_TexturePtr{};
		float m_Depth{};
		float m_Width{};
		float m_Height{};
		std::pair<bool, bool> m_FlipAxis{ false, false };
	};
}