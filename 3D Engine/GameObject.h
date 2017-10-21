#pragma once
#include "Component.h"
#include <vector>
#include <string>

class GameObject
{
public:
	void Update();
	Component* FindComponent(std::string name);

public:
	std::string name;
	GameObject* parent;

private:
	std::vector<Component*> components;
	std::vector<GameObject*> childrens;
};

