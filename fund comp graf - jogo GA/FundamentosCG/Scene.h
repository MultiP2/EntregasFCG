#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Scene
{
public:
	Scene(const std::string& name)
		:
		name(name)
	{}
	virtual void update(GLFWwindow* window, float dt) = 0;
	virtual void draw() = 0;
	const std::string& getName() const
	{
		return name;
	}
private:
	std::string name;
};