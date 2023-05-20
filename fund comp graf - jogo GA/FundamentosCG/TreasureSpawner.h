#pragma once

#include "GameObject.h"
#include "TileField.h"
#include <random>

class TreasureSpawner
{
public:
	TreasureSpawner(Shader* shader, 
		std::shared_ptr<TileField> tf, 
		const glm::ivec2& playerFieldPos,
		int numTreasures);
	void update(float dt);
	void draw() const;
	bool checkIfTreasureCaught(const glm::ivec2& pos);
	int getNumTreasures() const
	{
		return int(treasures.size());
	}
private:
	friend class Level;
	void applyCursedTreasureMark();
	void changeTreasuresPos();
	void setNumTreasures(int numTreasures);
private:
	struct Treasure
	{
		GameObject gameObject;
		glm::ivec2 fieldPos;
	};
private:
	std::mt19937 rng;
	std::uniform_int_distribution<int> spawnPosDist;

	Shader* shader;
	std::shared_ptr<TileField> tf;
	const glm::ivec2& playerFieldPos;

	float time = 0.0f;
	const float changePosRate = 6.0f;

	std::vector<unsigned int> indexSpawnableTiles;
	const float minSqrSpawnDist = 9.0f;

	std::string treasureSpritePath;
	std::vector<Treasure> treasures;
	std::vector<Model::Vertex> vertices = {
		//posições, cores e coordenadas de textura respectivamente
	   { { 0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f }}, //superior direito
	   { { 0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0 }}, //inferior direito
	   { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0 } }, //inferior esquerdo
	   { { -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } }  //superior esquerdo
	};
	std::vector<unsigned int> indices = {
	0, 1, 3, //primeiro triângulo
	1, 2, 3  //segundo triângulo
	};
	IndexedTriangleList<Model::Vertex> triangles{ vertices, indices };

	Tile cursedTile = {
		"../Tilesets/Ground_Rocky1_256x128.png",
		"cursed",
		shader,
		{ 0.0f, 0.0f },
		{ 0, 0 },
		{ 0.33333333333f, 0.2f },
		{ 0.0f, 0.2f } };
	std::vector<Tile> changedTiles{};
	std::vector<glm::ivec2> changedPositions{};
};