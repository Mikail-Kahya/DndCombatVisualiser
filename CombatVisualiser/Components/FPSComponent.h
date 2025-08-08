#pragma once

#include <vector>

#include "IComponent.h"

namespace mk
{
	class TextComponent;

	class FPSComponent : public IComponent
	{
	public:
		FPSComponent() = default;
		~FPSComponent() override = default;

		FPSComponent(const FPSComponent& other)				= delete;
		FPSComponent(FPSComponent&& other)					= delete;
		FPSComponent& operator=(const FPSComponent& other)	= delete;
		FPSComponent& operator=(FPSComponent&& other)		= delete;

		void Start() override;

		void Update() override;

		void SetPrecision(int precision);
		void SetUpdateDelay(float updateDelay);

	private:
		float GetAverageFPS();

		TextComponent* m_TextCompPtr{};

		bool m_NeedsUpdate{ true };
		int m_Precision{ 1 };
		float m_UpdateDelay{ 0.5f };
		float m_Timer{};

		std::vector<float> m_FrameRates{};
	};
}