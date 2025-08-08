#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "ResourceManager.h"
#include "Renderer.h"

using namespace mk;

void ResourceManager::Init(const std::filesystem::path& dataPath)
{
	m_DataPath = dataPath;

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

Texture2D* ResourceManager::LoadTexture(const std::string& file)
{
	// Check if texture was loaded before
	if (m_Textures.contains(file))
		return m_Textures.at(file).get();

	// Load new texture
	const auto fullPath = m_DataPath / file;
	auto texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.string().c_str());
	if (texture == nullptr)
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());

	m_Textures[file] = std::make_unique<Texture2D>(texture);
	return m_Textures.at(file).get();
}

Font* ResourceManager::LoadFont(const std::string& file, unsigned int size)
{
	const std::string alias{ file + std::to_string(size) };

	// Check if font was loaded before
	if (m_Fonts.contains(alias))
		return m_Fonts.at(alias).get();

	// Load new font
	const auto fullPath = m_DataPath / file;
	m_Fonts[alias] = std::make_unique<Font>(fullPath.string(), size);

	return m_Fonts.at(alias).get();
}