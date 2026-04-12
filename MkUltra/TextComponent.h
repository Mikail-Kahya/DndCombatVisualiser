#pragma once
#include <string>
#include <memory>

#include "IComponent.h"
#include "Texture2D.h"

namespace mk
{
	class SpriteComponent;
	class Texture2D;
	class Font;
	
	class TextComponent final : public IComponent
	{
	public:
		TextComponent(const std::string& text, const std::string& fontName = "Lingua.otf", int size = 11);
		~TextComponent() override = default;

		TextComponent(const TextComponent& other)				= delete;
		TextComponent(TextComponent&& other)					= delete;
		TextComponent& operator=(const TextComponent& other)	= delete;
		TextComponent& operator=(TextComponent&& other)			= delete;

		void Start() override;

		void Update() override;

		void SetText(const std::string& text);
		void SetFont(const std::string& fontPath, unsigned int size);
		void SetColor(const Color& color);
		void SetAnchor(const Vector2& anchor) const;

	private:
		bool m_NeedsUpdate{ false };
		std::string m_Text{};
		Color m_Color{};
		std::unique_ptr<Texture2D> m_Texture{};
		Font* m_FontPtr{};
		SpriteComponent* m_RendererPtr{};
	};
}
