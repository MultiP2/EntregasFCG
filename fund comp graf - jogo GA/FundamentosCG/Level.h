#pragma once

#include "Scene.h"
#include "IndexedTriangleList.h"
#include "TileField.h"
#include "Character.h"
#include "TreasureSpawner.h"

class Level : public Scene
{
public:
	Level(const std::string& name, const std::wstring& filePath, Shader* shader, class Game* game);
	Level(const std::string& name, Shader* shader, Game* game );
	void update(GLFWwindow* window, float dt) override;
	void draw() override;
private:
	void handleInput(GLFWwindow* window);
	void checkWinLoseCases();
	void reset();
	void endLevel();
private:
	Shader* shader;
	Game* game;
	const glm::ivec2 initialCharPos = { 0 , 0 };
	glm::ivec2 charFieldPos = initialCharPos;
	glm::vec2 charDir = { 0.0f , 0.0f };
	std::unique_ptr<Character> character;
	std::shared_ptr<TileField> tf;
	std::unique_ptr<TreasureSpawner> ts;
	int numTreasures = 0;
	float time = 0.0f;
	bool updateInput = true;
};