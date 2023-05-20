#include "TreasureSpawner.h"
#include <algorithm>

TreasureSpawner::TreasureSpawner(
	Shader* shader,
	std::shared_ptr<TileField> tf,
	const glm::ivec2& playerFieldPos,
	int numTreasures)
	:
	rng(std::random_device{}()),
	shader(shader),
	tf(tf),
	playerFieldPos(playerFieldPos),
	treasureSpritePath("../Textures/treasure.png")
{
	const auto numTiles = tf->getDimensions().x * tf->getDimensions().y;
	for (int i = 0; i < numTiles; i++)
	{
		if (tf->getTileType(i) == "ground")
		{
			indexSpawnableTiles.push_back(i);
		}
	}

	spawnPosDist = std::uniform_int_distribution<int>(0, int(indexSpawnableTiles.size()) - 1);
	setNumTreasures(numTreasures);
	changeTreasuresPos();
}
void TreasureSpawner::update(float dt)
{
	time += dt;

	if (time >= changePosRate)
	{
		applyCursedTreasureMark();
		changeTreasuresPos();
		time = 0.0f;
	}
}
void TreasureSpawner::draw() const
{
	for (auto& treasure : treasures)
	{
		shader->setVec2("offsets", 0.0f, 0.0f);
		treasure.gameObject.model->draw();
	}
}
bool TreasureSpawner::checkIfTreasureCaught(const glm::ivec2& pos)
{
	const auto nTreasuresCaught = std::erase_if(treasures, [&pos](const Treasure& t)
		{
			return t.fieldPos == pos;
		});

	return nTreasuresCaught > 0;
}
void TreasureSpawner::applyCursedTreasureMark()
{
	tf->changeTileTypes(changedPositions, changedTiles);

	std::vector< glm::ivec2> positions;
	changedTiles.clear();
	for (const auto& treasure : treasures)
	{
		positions.push_back(treasure.fieldPos);
		changedTiles.push_back(Tile(
			cursedTile.getTexPath(),
			cursedTile.getType(),
			shader,
			{ 0.0f, 0.0f },
			{ 0, 0 },
			cursedTile.getNorTexDim(),
			cursedTile.getTilesetOff()));
	}

	changedTiles = tf->changeTileTypes(positions, changedTiles);
	changedPositions = positions;
}
void TreasureSpawner::changeTreasuresPos()
{
	auto getSqrDist = [](const glm::ivec2& pos0, const glm::ivec2& pos1)
	{
		auto xDelta = float(pos1.x - pos0.x);
		auto yDelta = float(pos1.y - pos0.y);

		return xDelta * xDelta + yDelta * yDelta;
	};

	auto curIt = treasures.begin();
	for (auto& treasure : treasures)
	{
		do
		{
			const auto indexNewSpawn = indexSpawnableTiles[spawnPosDist(rng)];
			const auto xPos = indexNewSpawn % tf->getDimensions().x;
			const auto yPos = (indexNewSpawn - xPos) / tf->getDimensions().x;;
			treasure.fieldPos = { xPos, yPos };
			treasure.gameObject.transform.translation = { tf->getTilePosition(indexNewSpawn) - glm::vec2{0.0f, 8.0f}, 1.0f};
		} while (minSqrSpawnDist > getSqrDist(playerFieldPos, treasure.fieldPos) ||
				 curIt != std::find_if(treasures.begin(), curIt, [&treasure](const Treasure& t) {return t.fieldPos == treasure.fieldPos; }));
		curIt++;

		treasure.gameObject.model = std::make_shared<Model>(
			shader, 
			triangles,
			treasureSpritePath,
			treasure.gameObject.transform.mat4());
	}
}
void TreasureSpawner::setNumTreasures(int numTreasures)
{
	for (int i = treasures.size(); i < numTreasures; i++)
	{
		treasures.emplace_back(Treasure(GameObject::createGameObject(), { 0, 0 }));
		treasures[i].gameObject.transform.translation = { 0.0f, 0.0f , 1.0f };
		treasures[i].gameObject.transform.scale = { 30.0f, 30.0f, 1.0f };
		treasures[i].gameObject.model = std::make_shared<Model>(
			shader,
			triangles,
			treasureSpritePath,
			treasures[i].gameObject.transform.mat4());
	}
}
