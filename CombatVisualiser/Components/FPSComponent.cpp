#include <algorithm>
#include <numeric>

#include "FPSComponent.h"

#include "GameObject.h"
#include "SceneManager.h"
#include "TextComponent.h"
#include "Texture2D.h"

using namespace mk;

void FPSComponent::Start()
{
	m_TextCompPtr = GetOwner()->AddComponent<TextComponent>("", std::string{"Lingua.otf"}, 18);
}

void FPSComponent::Update()
{
	if (m_TextCompPtr == nullptr)
		return;

	if (m_NeedsUpdate)
	{
		m_NeedsUpdate = false;
		m_Timer -= m_UpdateDelay;

		// Update text component
		std::stringstream textBuffer;
		textBuffer << std::fixed << std::setprecision(m_Precision) << GetAverageFPS();
		textBuffer << " FPS";

		m_TextCompPtr->SetText(textBuffer.str());
		m_FrameRates.clear();

		return;
	}

	const TimeManager& time{ Time() };
	m_FrameRates.emplace_back(time.GetFPS());
	m_Timer += time.deltaTime;
	m_NeedsUpdate = m_Timer > m_UpdateDelay;
}

void FPSComponent::SetPrecision(int precision)
{
	m_Precision = precision;
}

void FPSComponent::SetUpdateDelay(float updateDelay)
{
	m_UpdateDelay = updateDelay;
}

float FPSComponent::GetAverageFPS()
{
	if (m_FrameRates.size() < 4)
		return Time().GetFPS();

	std::ranges::sort(m_FrameRates);
	return std::accumulate( m_FrameRates.begin() + 1, m_FrameRates.end() - 1, 0.f) / (m_FrameRates.size() - 2);
}