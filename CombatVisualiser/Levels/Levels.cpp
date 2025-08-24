#include "Levels.h"

#include <SDL_image.h>

#include "MkUltra.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Renderer.h"

#include "GameObject.h"
#include "InputManager.h"

#include "TextComponent.h"
#include "SpriteComponent.h"
#include "BoxColliderComponent.h"
#include "StateComponent.h"

#include "Components/Character/MovementComponent.h"
#include "Components/Character/HealthBarComponent.h"
#include "Components/Character/HealthComponent.h"
#include "Components/FPSComponent.h"

#include "ShapeComponent.h"
#include "Components/Map/ShapeGridComponent.h"

#include "Input/PlayerCommand.h"


using namespace mk;

GameObject* LoadButton(Scene& scene, const std::string& buttonText, const glm::vec2& pos, float width, float height);

void mk::LoadGameMenu(Scene& scene)
{
	const Renderer& renderer{ Renderer::GetInstance() };
	const int screenHeight{ renderer.GetHeight() };
	const int screenWidth{ renderer.GetWidth() };

	GameObject* titlePtr{ scene.SpawnObject("Title") };
	titlePtr->SetLocalPosition({ screenWidth / 2, screenHeight * 0.7f });

	TextComponent* textPtr = titlePtr->AddComponent<TextComponent>("TRON");
	textPtr->SetColor(Color{ 255,255,255,255 });
	textPtr->SetAnchor(glm::vec2{ 0.5f });
	textPtr->SetFont("Lingua.otf", 80);

	constexpr float buttonWidth{ 100.f };
	constexpr float buttonHeight{ buttonWidth * 0.5f };
	LoadButton(scene, "Start", glm::vec2{ screenWidth / 2, screenHeight * 0.52f }, buttonWidth, buttonHeight);
	LoadButton(scene, "Versus", glm::vec2{ screenWidth / 2, screenHeight * 0.4f }, buttonWidth, buttonHeight);

	//GameObject* menuStateSwitcher{ scene.SpawnObject("Menu state") };
	//menuStateSwitcher->AddComponent<StateComponent>();
}

GameObject* LoadButton(Scene& scene, const std::string& buttonText, const glm::vec2& pos, float width, float height)
{
	GameObject* buttonPtr{ scene.SpawnObject("Button") };
	buttonPtr->SetLocalPosition(pos);
	SpriteComponent* bgCompPtr{ buttonPtr->AddComponent<SpriteComponent>("Button.png") };
	bgCompPtr->SetAnchor(glm::vec2{ 0.5f });
	bgCompPtr->SetWidth(width);
	bgCompPtr->SetHeight(height);

	GameObject* buttonTextPtr{ scene.SpawnObject("ButtonText") };
	buttonTextPtr->SetParent(buttonPtr);
	TextComponent* textPtr = buttonTextPtr->AddComponent<TextComponent>(buttonText);
	textPtr->SetColor(Color{ 255,255,255,255 });
	textPtr->SetAnchor(glm::vec2{ 0.5f });
	textPtr->SetFont("Lingua.otf", 20);

	return buttonPtr;
}

void mk::LoadMainGame(Scene& scene)
{
	const Renderer& renderer{ Renderer::GetInstance() };
	const int screenHeight{ renderer.GetHeight() };

	GameObject* fps = scene.SpawnObject("fps");
	fps->SetLocalPosition({ 0, 0.95f * screenHeight });
	auto fpsComponent = fps->AddComponent<FPSComponent>();
	fpsComponent->SetUpdateDelay(0.5f);


	GameObject* bgGrid{ scene.SpawnObject("Shape") };
	auto shapeComponentPtr{ bgGrid->AddComponent<ShapeComponent>() };
	shapeComponentPtr->AddPoint({ -50, -50 });
	shapeComponentPtr->AddPoint({ 50, -50});
	shapeComponentPtr->AddPoint({ 0, 50 });
	shapeComponentPtr->SetColor({ 200, 0, 0, 255 });

	bgGrid->AddComponent<ShapeGridComponent>(5,5, 10.f);
}

void LoadHud(Scene& scene, const std::vector<GameObject*>& players)
{
	for (size_t idx{}; idx < players.size(); ++idx)
	{
		GameObject* playerPtr{ players[idx] };

		HealthComponent* healthCompPtr{ playerPtr->GetComponent<HealthComponent>() };

		GameObject* HUDWrapper{ scene.SpawnObject("Hud wrapper") };
		HUDWrapper->SetLocalPosition({ playerPtr->GetWorldPosition().x , 200 });

		GameObject* score{ scene.SpawnObject("score") };
		score->SetParent(HUDWrapper);
		TextComponent* textCompPtr{ score->AddComponent<TextComponent>("Score: 0", "Lingua.otf", 30) };
		textCompPtr->SetAnchor({ 0.5f, 0.5f });

		GameObject* health{ scene.SpawnObject("health") };
		health->SetParent(HUDWrapper);
		health->SetLocalPosition({ 0, 30 });
		textCompPtr = health->AddComponent<TextComponent>("Health: " + std::to_string(healthCompPtr->GetHealth()), "Lingua.otf", 20);
		textCompPtr->SetAnchor({ 0.5f, 0.5f });
		HealthBarComponent* healthBarCompPtr{ health->AddComponent<HealthBarComponent>() };

		healthCompPtr->AddObserver(healthBarCompPtr);
	}
}

/*
void LoadLevel(Scene& scene)
{
	SDL_Surface* surfacePtr = IMG_Load("./Data/Level00.png");
	uint8_t* pixels = static_cast<uint8_t*>(surfacePtr->pixels);


	std::vector<glm::vec2> playerSpawns{};
	std::vector<GameObject*> enemies{};

	for (int row{}; row < surfacePtr->h; ++row)
	{
		for (int col{}; col < surfacePtr->w; ++col) 
		{
			// surface height - row to read from bottom to top
			const int pixelIdx{ ((surfacePtr->h - row - 1) * surfacePtr->w + col) * surfacePtr->format->BytesPerPixel };
			uint8_t r = pixels[pixelIdx];
			uint8_t g = pixels[pixelIdx + 1];
			uint8_t b = pixels[pixelIdx + 2];

			const glm::vec2 pos{ col * TILE_SIZE, row * TILE_SIZE };

			if (g == 255)
				LoadTile(scene, TILE_SIZE, TILE_SIZE, pos);
			if (r == 255)
				enemies.emplace_back(LoadEnemy(scene, pos));
			if (b == 255)
				playerSpawns.emplace_back(pos + glm::vec2{ TILE_SIZE });
		}
	}
	SDL_FreeSurface(surfacePtr);

	// Ignore each other -> fix by using layers
	for (auto enemyPtr : enemies)
	{
		for (auto otherEnemyPtr : enemies)
		{
			if (enemyPtr == otherEnemyPtr)
				continue;

			enemyPtr->GetComponent<BoxColliderComponent>()->Ignore(otherEnemyPtr);
		}
	}

	
}
*/