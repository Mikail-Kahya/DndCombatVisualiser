#include <SDL.h>
#include "Texture2D.h"

mk::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_texture);
}

mk::Vector2 mk::Texture2D::GetSize() const
{
	return m_Size;
}

SDL_Texture* mk::Texture2D::GetSDLTexture() const
{
	return m_texture;
}

mk::Texture2D::Texture2D(SDL_Texture* texture)
{
	m_texture = texture;
	SDL_Rect dst;
	SDL_QueryTexture(texture, nullptr, nullptr, &dst.w, &dst.h);
	m_Size = { static_cast<float>(dst.w),static_cast<float>(dst.h), 0 };
}
