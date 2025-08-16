#include <stdexcept>
#include <cstring>
#include "imgui.h"

#include "Renderer.h"

#include "RenderComponent.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "GUI.h"
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

	m_Renderers.sort([](RenderComponent* a, RenderComponent* b)
		{
			return a->GetRenderDepth() < b->GetRenderDepth();
		});

	m_DepthChanged = false;
}

void Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_Renderer);

	for (RenderComponent* renderComponentPtr : m_Renderers)
	{
		if (renderComponentPtr->GetTexture() == nullptr)
			continue;
		RenderTexture(renderComponentPtr);
	}

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

void Renderer::RegisterRenderComponent(RenderComponent* renderComponentPtr)
{
	const auto foundIt = std::ranges::find(m_Renderers, renderComponentPtr);
	if (foundIt != m_Renderers.end())
		return;

	// place in right order to avoid full resort
	const float newDepth{ renderComponentPtr->GetRenderDepth() };

	const auto lowerBoundIt = std::ranges::find_if(m_Renderers,
		[newDepth](const RenderComponent* a)
		{
			const float aDepth{ a->GetRenderDepth() };
			return aDepth < newDepth;
		});
	

	if (lowerBoundIt == m_Renderers.end())
		m_Renderers.push_front(renderComponentPtr);
	else if (lowerBoundIt == m_Renderers.begin())
		m_Renderers.push_back(renderComponentPtr);
	else
		m_Renderers.insert(std::prev(lowerBoundIt), renderComponentPtr);
}

void Renderer::UnregisterRenderComponent(RenderComponent* renderComponentPtr)
{
	std::erase(m_Renderers, renderComponentPtr);
}

void Renderer::FlagDepthDirty()
{
	m_DepthChanged = true;
}

void Renderer::RenderTexture(const RenderComponent* renderComponentPtr) const
{
	const Texture2D& texture{ *renderComponentPtr->GetTexture() };
	const int width{ static_cast<int>(renderComponentPtr->GetWidth()) };
	const int height{ static_cast<int>(renderComponentPtr->GetHeight()) };
	const glm::vec2& anchor{ renderComponentPtr->GetAnchor() };
	const glm::vec2 position{ renderComponentPtr->GetRenderPosition() };
	const SDL_Rect dstRect{
		static_cast<int>(position.x),
		m_Height - static_cast<int>(position.y) - height, // flip Y to be at the bottom
		width,
		height
	};

	const glm::vec2& srcPos{ renderComponentPtr->GetSrcPosition() };
	const glm::vec2& srcSize{ renderComponentPtr->GetSrcSize() };
	const SDL_Rect srcRect{
		static_cast<int>(srcPos.x),
		static_cast<int>(srcPos.y),
		static_cast<int>(srcSize.x),
		static_cast<int>(srcSize.y)
	};

	const float angle{ -renderComponentPtr->GetOwner()->GetRotation() };
	const auto flipAxis{ renderComponentPtr->IsFlipped() };

	const SDL_Point pivot{
		static_cast<int>(dstRect.w * anchor.x),
		dstRect.h - static_cast<int>(dstRect.h * anchor.y) // flip Y to be at the bottom
	};

	SDL_RendererFlip flip{};
	if (flipAxis.first)
		flip = SDL_FLIP_HORIZONTAL;
	if (flipAxis.second)
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
