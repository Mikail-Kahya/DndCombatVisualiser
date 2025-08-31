#include <stdexcept>
#include <cstring>
#include "imgui.h"

#include "Renderer.h"

#include "SpriteComponent.h"
#include "ShapeComponent.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "GUI.h"
#include "IRenderable.h"
#include "PhysicsSystem.h"

using namespace mk;

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void Renderer::Init(int width, int height)
{
	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_OPENGL
	);
	m_Width = width;
	m_Height = height;

	if (m_Window == nullptr)
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());

	m_Renderer = SDL_CreateRenderer(m_Window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr) 
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	GUI::GetInstance().Init(m_Window, GetSDLRenderer());
}

void Renderer::Update()
{
	GUI::GetInstance().Update();

	if (!m_DepthChanged)
		return;

	m_Renderers.sort([](const IRenderable* a, const IRenderable* b)
		{
			return a->GetDepth() < b->GetDepth();
		});

	m_DepthChanged = false;
}

void Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_Renderer);

	for (IRenderable* rendererPtr : m_Renderers)
		if (rendererPtr->IsVisible()) rendererPtr->Render();

	GUI::GetInstance().Render();

#ifndef NDEBUG
	PhysicsSystem::GetInstance().DrawDebug();
#endif


	SDL_RenderPresent(m_Renderer);
}

void Renderer::Destroy()
{
	GUI::GetInstance().Destroy();
	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
	if (m_Window != nullptr)
	{
		SDL_DestroyWindow(m_Window);
		m_Window = nullptr;
	}
}

void Renderer::RenderRect(int x, int y, int width, int height, const Color& color) const
{
	const SDL_Rect rect{ x, m_Height - y - height, width, height };
	Color temp{};
	SDL_GetRenderDrawColor(m_Renderer, &temp.r, &temp.g, &temp.b, &temp.a);

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(m_Renderer , &rect);

	// Reset color
	SDL_SetRenderDrawColor(m_Renderer, temp.r, temp.g, temp.b, temp.a);
}

void Renderer::RenderLine(int x1, int y1, int x2, int y2, const Color& color) const
{
	Color temp{};
	SDL_GetRenderDrawColor(m_Renderer, &temp.r, &temp.g, &temp.b, &temp.a);

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(m_Renderer, x1, y1, x2, y2);

	// Reset color
	SDL_SetRenderDrawColor(m_Renderer, temp.r, temp.g, temp.b, temp.a);
}

void Renderer::RenderShape(const std::vector<glm::vec2>& points, const Color& color) const
{
	Color temp{};
	SDL_GetRenderDrawColor(m_Renderer, &temp.r, &temp.g, &temp.b, &temp.a);

	// Transform to sdl points
	std::vector<SDL_FPoint> sdlPoints(points.size());
	for (size_t idx{}; idx < sdlPoints.size(); ++idx)
		sdlPoints[idx] = { points[idx].x, points[idx].y };

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLinesF(m_Renderer, sdlPoints.data(), static_cast<int32_t>(sdlPoints.size()));

	// Reset
	SDL_SetRenderDrawColor(m_Renderer, temp.r, temp.g, temp.b, temp.a);
}

void Renderer::RenderPoint(const glm::vec2& point, const Color& color) const
{
	Color temp{};
	SDL_GetRenderDrawColor(m_Renderer, &temp.r, &temp.g, &temp.b, &temp.a);

	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawPointF(m_Renderer, point.x, point.y);

	// Reset
	SDL_SetRenderDrawColor(m_Renderer, temp.r, temp.g, temp.b, temp.a);
}

float Renderer::GetNextDepth()
{
	constexpr float depthSlice{ 0.1f };
	const float depth{ m_AutoDepth };
	m_AutoDepth += depthSlice;
	return depth;
}

SDL_Renderer* Renderer::GetSDLRenderer() const { return m_Renderer; }

int Renderer::GetHeight() const noexcept
{
	return m_Height;
}

int Renderer::GetWidth() const noexcept
{
	return m_Width;
}

void Renderer::RegisterRenderer(IRenderable* rendererPtr)
{
	const auto foundIt = std::ranges::find(m_Renderers, rendererPtr);
	if (foundIt != m_Renderers.end())
		return;

	// place in right order to avoid full resort
	const float newDepth{ rendererPtr->GetDepth() };

	const auto lowerBoundIt = std::ranges::find_if(m_Renderers,
		[newDepth](const IRenderable* a)
		{
			const float aDepth{ a->GetDepth() };
			return aDepth < newDepth;
		});
	

	if (lowerBoundIt == m_Renderers.end())
		m_Renderers.push_front(rendererPtr);
	else if (lowerBoundIt == m_Renderers.begin())
		m_Renderers.push_back(rendererPtr);
	else
		m_Renderers.insert(std::prev(lowerBoundIt), rendererPtr);
}

void Renderer::DeregisterRenderer(IRenderable* rendererPtr)
{
	std::erase(m_Renderers, rendererPtr);
}

void Renderer::FlagDepthDirty()
{
	m_DepthChanged = true;
}

void Renderer::RenderTexture(	const Texture2D& texture, int width, int height, 
								const glm::vec2& pos, const glm::vec2& anchor, float angle, 
								const glm::vec2& srcPos, int srcWidth, int srcHeight, 
								bool flipX, bool flipY) const
{
	if (!texture.GetSDLTexture())
		return;

	const SDL_Rect dstRect{
		static_cast<int>(pos.x),
		m_Height - static_cast<int>(pos.y) - height, // flip Y to be at the bottom
		width,
		height
	};


	const SDL_Rect srcRect{
		static_cast<int>(srcPos.x),
		static_cast<int>(srcPos.y),
		srcWidth, srcHeight
	};


	const SDL_Point pivot{
		static_cast<int>(dstRect.w * anchor.x),
		dstRect.h - static_cast<int>(dstRect.h * anchor.y) // flip Y to be at the bottom
	};

	SDL_RendererFlip flip{};
	if (flipX)
		flip = SDL_FLIP_HORIZONTAL;
	if (flipY)
		flip = static_cast<SDL_RendererFlip>(flip | SDL_FLIP_VERTICAL);

	SDL_RenderCopyEx(m_Renderer, texture.GetSDLTexture(), &srcRect, &dstRect, angle, &pivot, flip);
}


SDL_Rect Renderer::GetDstRect(const Texture2D& texture, float x, float y) const
{
	const auto size{ texture.GetSize() };
	return{	static_cast<int>(x),
				m_Height - static_cast<int>(y) - texture.GetSize().y, // flip Y to be at the bottom
				size.x,
				size.y };
}
