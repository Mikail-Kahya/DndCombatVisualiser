#pragma once
#include <concepts>
#include <memory>
#include <string>
#include <vector>

#include "ISingleton.h"
#include "GUIWidget.h"
#include "SDL_events.h"

// design inspired by Matias Devred

struct ImGuiIO;
struct SDL_Window;
struct SDL_Renderer;

namespace mk
{
	class GUI final : public ISingleton<GUI>
	{
		friend class ISingleton<GUI>;
	public:
		~GUI() override = default;

		GUI(const GUI& other)					= delete;
		GUI(GUI&& other) noexcept				= delete;
		GUI& operator=(const GUI& other)		= delete;
		GUI& operator=(GUI&& other)	noexcept	= delete;

		void Init(SDL_Window* windowPtr, SDL_Renderer* rendererPtr);
		bool ProcessSDLEvents(const SDL_Event& event);
		void Update();
		void Render() const;
		void Destroy();

		template<class WidgetType, typename... Args>
		requires(std::derived_from<WidgetType, GUIWidget>)
		[[nodiscard]] WidgetType* Add(const std::string& widgetName, const Args&... args);

	private:
		GUI() = default;

		void FlushBuffer();
		void DestroyWidgets();
		void BeginFrame() const;
		void EndFrame() const;

		std::vector<std::unique_ptr<GUIWidget>> m_Widgets{};
		std::vector<std::unique_ptr<GUIWidget>> m_WidgetBuffer{};
		ImGuiIO* m_Io{};
	};

	template <class WidgetType, typename ... Args>
	requires (std::derived_from<WidgetType, GUIWidget>)
	WidgetType* GUI::Add(const std::string& widgetName, const Args&... args)
	{
		std::unique_ptr<WidgetType> widget{ std::make_unique<WidgetType>(widgetName, args...) };
		WidgetType* widgetPtr{ widget.get() };

		m_WidgetBuffer.emplace_back(std::move(widget));
		return widgetPtr;
	}
}
