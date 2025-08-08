#pragma once
#include <filesystem>
#include <string>
#include <memory>
#include <map>

#include "Texture2D.h"
#include "Font.h"
#include "ISingleton.h"

namespace mk
{

	class ResourceManager final : public ISingleton<ResourceManager>
	{
		friend class ISingleton<ResourceManager>;
	public:
		~ResourceManager() override = default;

		ResourceManager(const ResourceManager& other)				= delete;
		ResourceManager(ResourceManager&& other)					= delete;
		ResourceManager& operator=(const ResourceManager& other)	= delete;
		ResourceManager& operator=(ResourceManager&& other)			= delete;

		void Init(const std::filesystem::path& data);

		Texture2D* LoadTexture(const std::string& file);
		Font* LoadFont(const std::string& file, unsigned int size);

	private:
		ResourceManager() = default;
		std::filesystem::path m_DataPath;

		std::map<std::string, std::unique_ptr<Texture2D>> m_Textures{};
		std::map<std::string, std::unique_ptr<Font>> m_Fonts{};
	};
}