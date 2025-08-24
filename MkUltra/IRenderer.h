#pragma once
namespace mk
{
	class IRenderer
	{
	public:
		IRenderer();
		virtual ~IRenderer();
		IRenderer(const IRenderer& other) = delete;
		IRenderer(IRenderer&& other) noexcept = delete;
		IRenderer& operator=(const IRenderer& other) = delete;
		IRenderer& operator=(IRenderer&& other) noexcept = delete;


		virtual void Render() const = 0;

		void SetDepth(float newDepth);
		void SetVisibility(bool isVisible);
		float GetDepth() const;
		bool IsVisible() const;

	private:
		float m_Depth{};
		bool m_IsVisible{ true };
	};
}