#include "Character.h"
#include "TileField.h"

Character::Character(Shader* shader, const glm::vec2& pos, const glm::vec2& scale)
	:
	shader(shader),
	spritePath("../Textures/character_sheet.png"),
	gameObject(GameObject::createGameObject())
{
	gameObject.transform.translation = glm::vec3(pos.x, pos.y, 1.0f);
	gameObject.transform.scale = glm::vec3(scale.x, scale.y, 1.0f);

	float dx = 1.0f / (float)nFrames;
	float dy = 1.0f / (float)nAnimations;
	
	for (int i = 0; i < (int)Sequence::StandingDown; i++)
	{
		animations.emplace_back(Animation(shader, gameObject.model, triangles, 0.0f, dy * i, dx, 0.0f, 4, spritePath, 0.16f));
	}
	for (int i = (int)Sequence::StandingDown; i < (int)Sequence::Count; i++)
	{
		animations.emplace_back(Animation(shader, gameObject.model, triangles, 0.0f, dy * i, 0.0f, 0.0f, 1, spritePath, 0.16f));
	}
}
void Character::update(float dt)
{
	gameObject.transform.translation += glm::vec3{ vel.x * dt, vel.y * dt, 0.0f };
	animations[(int)iCurSequence].update(dt, gameObject.transform.mat4());
}
void Character::draw() const
{
	animations[(int)iCurSequence].draw();
}
void Character::setDirection(const glm::vec2& dir)
{
	if (std::abs(dir.x) > std::abs(dir.y))
	{
		if (dir.x > 0.0f)
		{
			iCurSequence = Sequence::WalkingRight;
		}
		else if (dir.x < 0.0f)
		{
			iCurSequence = Sequence::WalkingLeft;
		}
	}
	else if (std::abs(dir.y) > std::abs(dir.x))
	{
		if (dir.y > 0.0f && dir.y > dir.x)
		{
			iCurSequence = Sequence::WalkingUp;
		}
		else if (dir.y < 0.0f)
		{
			iCurSequence = Sequence::WalkingDown;
		}
	}
	else
	{
		if (std::abs(vel.x) > std::abs(vel.y))
		{
			if (vel.x > 0.0f)
			{
				iCurSequence = Sequence::StandingRight;
			}
			else if (vel.x < 0.0f)
			{
				iCurSequence = Sequence::StandingLeft;
			}
		}
		else
		{
			if (vel.y < 0.0f)
			{
				iCurSequence = Sequence::StandingDown;
			}
			else if (vel.y > 0.0f)
			{
				iCurSequence = Sequence::StandingUp;
			}
		}
	}
	vel = dir * speed;
}

// Pedro Tavares Rodrigues