#pragma once

#include "Level.h"

class Game
{
public:
	Game(GLFWwindow* window, Shader* shader)
		:
		window(window)
	{
		scenes.push_back(std::make_unique<Level>("Level_1", L"Levels/Level1.txt", shader, this));
		scenes.push_back(std::make_unique<Level>("Level_2", L"Levels/Level2.txt", shader, this));
		scenes.push_back(std::make_unique<Level>("Level_3", L"Levels/Level3.txt", shader, this));
		curScene = scenes.begin();
	}
	void update(GLFWwindow* window, float dt)
	{
		(*curScene)->update(window, dt);
	}
	void draw() const
	{
		(*curScene)->draw();
	}
	void changeScene()
	{
		(std::next(curScene) == scenes.end()) ? curScene = scenes.begin() : curScene++;
		update(window, 0.0f);
	}
private:
	GLFWwindow* window;
	std::vector<std::unique_ptr<Scene>> scenes;
	std::vector<std::unique_ptr<Scene>>::iterator curScene;
};