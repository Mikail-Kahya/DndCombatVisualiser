#pragma once
#include <glm/vec2.hpp>

struct Color
{
	uint8_t r{ 255 };
	uint8_t g{ 255 };
	uint8_t b{ 255 };
	uint8_t a{ 255 };
};

struct SDL_Texture;
namespace mk
{
	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class Texture2D final
	{
	public:
		SDL_Texture* GetSDLTexture() const;
		explicit Texture2D(SDL_Texture* texture);
		~Texture2D();

		glm::ivec2 GetSize() const;

		Texture2D(const Texture2D &) = delete;
		Texture2D(Texture2D &&) = delete;
		Texture2D & operator= (const Texture2D &) = delete;
		Texture2D & operator= (const Texture2D &&) = delete;
	private:
		SDL_Texture* m_texture;
	};
}
