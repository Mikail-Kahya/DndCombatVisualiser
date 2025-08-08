#include "GUI.h"

#include <algorithm>

#include "imgui.h"
#include "SDL_video.h"
#include "SDL_render.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

void mk::GUI::Init(SDL_Window* windowPtr, SDL_Renderer* rendererPtr)
{
	// Setup ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	m_Io = &ImGui::GetIO();

	// Setup rendering
	ImGui_ImplSDL2_InitForOpenGL(windowPtr, SDL_GL_GetCurrentContext());
	ImGui_ImplSDLRenderer2_Init(rendererPtr);

	// Set style
	ImGui::StyleColorsDark();
}

bool mk::GUI::ProcessSDLEvents(const SDL_Event& event)
{
	ImGui_ImplSDL2_ProcessEvent(&event);
	return m_Io->WantCaptureMouse || m_Io->WantCaptureKeyboard;
}

void mk::GUI::Update()
{
	FlushBuffer();
	DestroyWidgets();
}

void mk::GUI::Render() const
{
	BeginFrame();
	for (const auto& widget : m_Widgets)
		widget->Render();
	EndFrame();
}

void mk::GUI::FlushBuffer()
{
	if (m_WidgetBuffer.empty())
		return;

	for (std::unique_ptr<GUIWidget>& bufferedWidget : m_WidgetBuffer)
		m_Widgets.emplace_back(std::move(bufferedWidget));

	m_WidgetBuffer.clear();
}

void mk::GUI::DestroyWidgets()
{
	// Remove destroy flagged components
	auto eraseIt = std::stable_partition(m_Widgets.begin(), m_Widgets.end(), [](const std::unique_ptr<GUIWidget>& widget)
		{
			return !widget->DestroyFlagged();
		});

	m_Widgets.erase(eraseIt, m_Widgets.end());
}

void mk::GUI::BeginFrame() const
{
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void mk::GUI::EndFrame() const
{
	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
}

void mk::GUI::Destroy()
{
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
