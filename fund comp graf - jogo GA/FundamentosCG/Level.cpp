#include "Level.h"
#include "Game.h"
#include <iostream>
#include <fstream>
Level::Level(const std::string& name, const std::wstring& filePath, Shader* shader, Game* game)
	:
	Scene(name),
	shader(shader),
	game(game)
{
	std::ifstream file(filePath);
	if (file.is_open())
	{
		auto ignoreLines = [](size_t numLines, std::ifstream& file)
		{
			std::string garbage;
			for (size_t i = 0; i < numLines; i++)
			{
				std::getline(file, garbage);
			}
		};

		ignoreLines(1, file);
		std::string line;
		std::getline(file, line);
		int numDiffTiles = std::stoi(line);
		
		ignoreLines(2, file);
		std::vector<std::string> tilesTypes;
		for (int i = 0; i < numDiffTiles; i++)
		{
			std::getline(file, line);
			tilesTypes.push_back(line);
		}

		ignoreLines(2, file);
		std::vector<std::string> tilesetPaths;
		for (int i = 0; i < numDiffTiles; i++)
		{
			std::getline(file, line);
			tilesetPaths.push_back(line);
		}

		ignoreLines(2, file);
		std::vector<glm::vec2> normalizedTexturesDimensions;
		for (int i = 0; i < numDiffTiles; i++)
		{
			std::getline(file, line);
			normalizedTexturesDimensions.push_back(
				{ std::stof(line.substr(0, line.find_first_of(","))),
				      std::stof(line.substr(line.find_first_of(" "), line.find_first_of("\n"))) });
		}

		ignoreLines(2, file);
		std::vector<glm::vec2> tilesetOffsets;
		for (int i = 0; i < numDiffTiles; i++)
		{
			std::getline(file, line);
			tilesetOffsets.push_back(
				{ std::stof(line.substr(0, line.find_first_of(","))),
					  std::stof(line.substr(line.find_first_of(" "), line.find_first_of("\n"))) });
		}

		ignoreLines(2, file);
		glm::ivec2 tileFieldDimensions{};
		std::getline(file, line);
		tileFieldDimensions.x = std::stoi(line.substr(0, line.find_first_of(",")));
		tileFieldDimensions.y = std::stoi(line.substr(line.find_first_of(" "), line.find_first_of("\n")));

		ignoreLines(2, file);
		std::vector<int> tileMap;
		for (int y = 0; y < tileFieldDimensions.y; y++)
		{
			for (int x = 0; x < tileFieldDimensions.x; x++)
			{
				tileMap.push_back(file.get() - '0');
				file.get();
			}
		}

		ignoreLines(2, file);
		std::getline(file, line);
		numTreasures = std::stoi(line);

		file.close();

		tf = std::make_shared<TileField>(
			std::move(tileFieldDimensions),
			glm::vec2{ 480.0f, 180.0f },
			std::move(tileMap),
			std::move(tilesTypes),
			std::move(tilesetPaths),
			std::move(normalizedTexturesDimensions),
			std::move(tilesetOffsets),
			shader);

		auto pos = tf->getTilePosition(initialCharPos.y * tf->getDimensions().x + initialCharPos.x);
		character = std::make_unique<Character>(shader, pos, glm::vec2{ 60.0f, 75.0f });

		ts = std::make_unique<TreasureSpawner>(shader, tf, charFieldPos, numTreasures);
	}
}
Level::Level(const std::string& name, Shader* shader, Game* game)
	:
	Scene(name),
	shader(shader),
	game(game),
	numTreasures(1)
{
	std::vector<int> tileMap;

	tileMap.insert(tileMap.end(),
		{ 1, 1, 1, 1, 0, 0, 1, 1, 1, 1,
		  1, 1, 0, 1, 0, 0, 1, 0, 1, 1,
		  1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
		  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		  0, 0, 0, 1, 0, 0, 1, 0, 0, 0,
		  0, 0, 0, 1, 0, 0, 1, 0, 0, 0,
		  1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		  1, 0, 0, 1, 0, 1, 1, 0, 0, 1,
		  1, 1, 0, 1, 0, 0, 1, 0, 1, 1,
		  1, 1, 1, 1, 0, 0, 1, 1, 1, 1 });
	/*
	
	*/

	std::vector<std::string> tilesTypes = {
		"water",
		"ground"
	};
	std::vector<std::string> tilesetPaths = {
		"../Tilesets/Solid_Tiles_Flat_128x88.png", 
		"../Tilesets/Ground_Rocky1_256x128.png"
	};
	std::vector<glm::vec2> normalizedTexturesDimensions = { 
		{ 1.0f / 6.0f, 1.0f },
		{ 1.0f / 3.0f, 1.0f / 5.0f }
	};
	std::vector<glm::vec2> tilesetOffsets = { 
		{ 5.0f / 6.0f, 0.0f },
		{ 2.0f / 3.0f, 3.0f / 5.0f }
	};
	
	tf = std::make_shared<TileField>(
		   glm::ivec2{ 10, 10 },
		   glm::vec2{ 480.0f, 180.0f },
		   std::move(tileMap),
		   std::move(tilesTypes),
		   std::move(tilesetPaths),
		   std::move(normalizedTexturesDimensions),
		   std::move(tilesetOffsets),
		   shader );

	auto pos = tf->getTilePosition(initialCharPos.y * tf->getDimensions().x + initialCharPos.x);
	character = std::make_unique<Character>(shader, pos, glm::vec2{60.0f, 75.0f});

	ts = std::make_unique<TreasureSpawner>(shader, tf, charFieldPos, numTreasures);
}
void Level::update(GLFWwindow* window, float dt)
{
	time += dt;
	if (updateInput)
	{
		handleInput(window);
	}
	else if (ts->getNumTreasures() <= 0)
	{
		endLevel();
	}
	else
	{
		reset();
	}
	ts->update(dt);
	character->update(dt);
}
void Level::draw()
{
	tf->draw();
	ts->draw();
	character->draw();
}
void Level::handleInput(GLFWwindow* window)
{
	const auto holdTime = 0.5f;
	const auto prevPos = charFieldPos;
	const auto xDimField = tf->getDimensions().x;
	const auto yDimField = tf->getDimensions().y;

	//direita
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && 
		charFieldPos.x < xDimField - 1 && charFieldPos.y > 0
		&& time > holdTime)
		charFieldPos += glm::ivec2{ 1, -1 };
	//esquerda
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && 
		charFieldPos.x > 0 && charFieldPos.y < yDimField - 1
		&& time > holdTime)
		charFieldPos += glm::ivec2{ -1, 1 };
	//cima
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS &&
		charFieldPos.x < xDimField - 1 && charFieldPos.y < yDimField - 1
		&& time > holdTime)
		charFieldPos += glm::ivec2{ 1, 1 };
	//baixo
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS &&
		charFieldPos.x > 0 && charFieldPos.y > 0
		&& time > holdTime)
		charFieldPos += glm::ivec2{ -1, -1 };

	auto index = charFieldPos.y * xDimField + charFieldPos.x;
	if (charFieldPos != prevPos)
	{
		charDir = glm::normalize(tf->getTilePosition(index) - character->getPosition());
		time = 0.0f;
	}
	if (glm::dot(tf->getTilePosition(index) - character->getPosition(), charDir) < 0)
	{
		charDir = { 0.0f, 0.0f };
	}

	character->setDirection(charDir);
	checkWinLoseCases();
}
void Level::checkWinLoseCases()
{
	if (tf->getTileType(charFieldPos.y * tf->getDimensions().x + charFieldPos.x) != "ground")
	{
		reset();
	}
	else if (ts->checkIfTreasureCaught(charFieldPos))
	{
		if (ts->getNumTreasures() <= 0)
		{
			endLevel();
		}
	}
}
void Level::reset()
{
	if (updateInput)
	{
		updateInput = false;
		time = 0.0f;
	}
	if (charDir != glm::vec2{ 0.0f, 0.0f } &&
		glm::dot(tf->getTilePosition(charFieldPos.y * tf->getDimensions().x + charFieldPos.x) - character->getPosition(), charDir) < 0)
	{
		charDir = { 0.0f, 0.0f };
		character->setDirection(charDir);
	}
	else if (time >= 2.0f)
	{
		updateInput = true;
		time = 0.0f;
		charFieldPos = initialCharPos;
		character->setPosition(tf->getTilePosition(charFieldPos.y * tf->getDimensions().x + charFieldPos.x));
		character->setDirection({ 0.0f, 1.0f });
		character->setDirection(charDir);
		ts->setNumTreasures(numTreasures);
		ts->changeTreasuresPos();
	}
}
void Level::endLevel()
{
	if (updateInput)
	{
		updateInput = false;
		time = 0.0f;
	}
	if (charDir != glm::vec2{ 0.0f, 0.0f } &&
		glm::dot(tf->getTilePosition(charFieldPos.y * tf->getDimensions().x + charFieldPos.x) - character->getPosition(), charDir) < 0)
	{
		charDir = { 0.0f, 0.0f };
		character->setDirection(charDir);
	}
	else if (time >= 2.0f)
	{
		reset();
		game->changeScene();
	}
}