#pragma once

#include "GameObject.h"

class Tile
{
public:
	Tile(const std::string& tilesetPath, const std::string& type,
		Shader* shader, const glm::vec2& pos, const glm::vec2& scale,
		const glm::vec2& normalizedTileTextureDimensions, const glm::vec2& tilesetOffsets);
	void draw() const;
	glm::vec2 getPosition() const
	{
		return glm::vec2(gameObject.transform.translation);
	}
	const std::string& getType() const
	{
		return type;
	}
	const std::string& getTexPath() const
	{
		return tilesetPath;
	}
	const glm::vec2& getNorTexDim() const
	{
		return normalizedTileTextureDimensions;
	}
	const glm::vec2& getTilesetOff() const
	{
		return tilesetOffsets;
	}
private:
	std::string tilesetPath;
	std::string type;
	Shader* shader;
	GameObject gameObject;
	glm::vec2 normalizedTileTextureDimensions;
	glm::vec2 tilesetOffsets;
	std::vector<Model::Vertex> vertices = {
		//posições, cores e coordenadas de textura respectivamente
	   { { 0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { normalizedTileTextureDimensions.x, 0.0f }}, //superior direito
	   { { 0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { normalizedTileTextureDimensions.x, normalizedTileTextureDimensions.y }}, //inferior direito
	   { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, normalizedTileTextureDimensions.y } }, //inferior esquerdo
	   { { -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } }  //superior esquerdo
	};
	std::vector<unsigned int> indices = {
	0, 1, 3, //primeiro triângulo
	1, 2, 3  //segundo triângulo
	};
	IndexedTriangleList<Model::Vertex> triangles{ vertices, indices };
};