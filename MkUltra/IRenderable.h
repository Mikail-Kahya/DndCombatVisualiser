#pragma once
namespace mk
{
	class IRenderable
	{
	public:
		IRenderable();
		virtual ~IRenderable();
		IRenderable(const IRenderable& other) = delete;
		IRenderable(IRenderable&& other) noexcept = delete;
		IRenderable& operator=(const IRenderable& other) = delete;
		IRenderable& operator=(IRenderable&& other) noexcept = delete;


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