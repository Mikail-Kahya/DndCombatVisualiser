#include <SDL.h>
#include "Texture2D.h"

mk::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_texture);
}

glm::ivec2 mk::Texture2D::GetSize() const
{
	SDL_Rect dst;
	SDL_QueryTexture(GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	return { dst.w,dst.h };
}

SDL_Texture* mk::Texture2D::GetSDLTexture() const
{
	return m_texture;
}

mk::Texture2D::Texture2D(SDL_Texture* texture)
{
	m_texture = texture;
}
