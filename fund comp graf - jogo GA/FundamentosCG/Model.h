#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Shader.h"
#include "IndexedTriangleList.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
class Model
{
public:
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 textCoodinates;
	};
public:
	Model(Shader* shader, const IndexedTriangleList<Vertex>& triangles, const std::string& texturePath, const glm::mat4& transformationMatrix);
	~Model();
	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;
	void draw();
private:
	void setupSprite(const IndexedTriangleList<Vertex>& triangles);
	void loadTexture(const std::string& path);
private:
	GLuint VAO = 0;
	GLuint texID;
	Shader* shader;
	glm::mat4 transformationMatrix;
};