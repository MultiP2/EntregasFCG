#include "Tile.h"
#include <memory>

Tile::Tile(const std::string& tilesetPath, const std::string& type,
	Shader* shader, const glm::vec2& pos, const glm::vec2& scale,
	const glm::vec2& normalizedTileTextureDimensions, const glm::vec2& tilesetOffsets)
	:
	tilesetPath(tilesetPath),
	type(type),
	shader(shader),
	gameObject(GameObject::createGameObject()),
	normalizedTileTextureDimensions(normalizedTileTextureDimensions),
	tilesetOffsets(tilesetOffsets)
{
	gameObject.transform.translation = { pos, 1.0f };
	gameObject.transform.scale = { scale, 1.0f };
	gameObject.model = std::make_shared<Model>(
		shader,
		triangles,
		tilesetPath,
		gameObject.transform.mat4());
}
void Tile::draw() const
{
	shader->setVec2("offsets", tilesetOffsets.x, tilesetOffsets.y);
	gameObject.model->draw();
}