#pragma once

#include "Animation.h"

class Character
{
private:
	enum class Sequence
	{
		WalkingDown,
		WalkingLeft,
		WalkingRight,
		WalkingUp,
		StandingDown,
		StandingLeft,
		StandingRight,
		StandingUp,
		Count
	};
public:
	Character(Shader* shader, const glm::vec2& pos, const glm::vec2& scale);
	void update(float dt);
	void draw() const;
	void setDirection(const glm::vec2& dir);
	glm::vec2 getPosition() const
	{
		return glm::vec2(gameObject.transform.translation);
	}
	void setPosition(const glm::vec2 pos)
	{
		gameObject.transform.translation = { pos, 1.0f };
	}
private:
	Shader* shader;
	std::string spritePath;
	glm::vec2 vel = { 0.0f, 0.0f };
	int nFrames = 4;
	int nAnimations = 4;
	std::vector<Animation> animations;
	Sequence iCurSequence = Sequence::StandingUp;
	float speed = 100.0f;
	GameObject gameObject;
	std::vector<Model::Vertex> vertices = {
		//posições, cores e coordenadas de textura respectivamente
	   { { 0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f / nFrames, 0.0f }}, //superior direito
	   { { 0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f / nFrames, 1.0 / nAnimations }}, //inferior direito
	   { { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0 / nAnimations } }, //inferior esquerdo
	   { { -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } }  //superior esquerdo
	};
	std::vector<unsigned int> indices = {
	0, 1, 3, //primeiro triângulo
	1, 2, 3  //segundo triângulo
	};
	IndexedTriangleList<Model::Vertex> triangles{ vertices, indices };
};