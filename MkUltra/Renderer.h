#pragma once
#include <list>
#include <SDL.h>
#include <vector>

#include "ISingleton.h"
#include "Texture2D.h"

namespace mk
{
	class RenderComponent;

	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public ISingleton<Renderer>
	{
		friend class ISingleton<Renderer>;
	public:
		void Init(int width, int height);
		void Update();
		void Render() const;
		void Destroy();

		void RenderRect(int x, int y, int width, int height, const Color& color) const;
		void RenderLine(int x1, int y1, int x2, int y2, const Color& color) const;
		void RenderShape(const std::vector<std::pair<int, int>>& points, const Color& color) const;

		float GetNextDepth();
		SDL_Renderer* GetSDLRenderer() const;
		int GetHeight() const noexcept;
		int GetWidth() const noexcept;

		const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }

		void RegisterRenderComponent(RenderComponent* renderComponentPtr);
		void UnregisterRenderComponent(RenderComponent* renderComponentPtr);
		void FlagDepthDirty();

	private:
		Renderer() = default;
		void RenderTexture(const RenderComponent* renderComponentPtr) const;
		SDL_Rect GetDstRect(const Texture2D& texture, float x, float y) const;

		// Sorts by float. Whenever the float changes
		std::list<RenderComponent*> m_Renderers{};

		SDL_Renderer* m_Renderer{};
		SDL_Window* m_Window{};
		SDL_Color m_ClearColor{};

		float m_AutoDepth{};
		bool m_DepthChanged{ false };
		int m_Width{};
		int m_Height{};
	};
}

