#pragma once

#include "GameObject.h"
#include <array>

class Animation
{
public:
	Animation(Shader* shader, std::shared_ptr<Model> model, const IndexedTriangleList<Model::Vertex>& triangles,
		float x_first, float y_first, float dx , float dy, int count, const std::string& spritePath,
		float holdTime, glm::vec3 chroma = {1.0f, 0.0f, 1.0f});
	void draw() const;
	void update(float dt, const glm::mat4& transformationMatrix);
private:
	void advance();
private:
	Shader* shader;
	std::shared_ptr<Model> model;
	const IndexedTriangleList<Model::Vertex>& triangles;
	glm::vec3 chroma;
	const std::string& spritePath;
	std::vector<glm::vec2> framesOffsets;
	int iCurFrame = 0;
	float holdTime;
	float curFrameTime = 0.0f;
};