#pragma once
#include "Component.h"
#include "Globals.h"
#include <vector>
#include <string>

class GameObject
{
public:
	GameObject(std::string name, GameObject* parent);
	~GameObject();

	void Draw();
	void Update();

	Component* FindComponent(component_type type);

	void AddChildren(GameObject* children);
	void AddComponent(Component* component);
	void DeleteComponentType(component_type type);
	void CreateHierarchy();

public:
	std::string name;
	GameObject* parent;

private:
	std::vector<Component*> components;
	std::vector<GameObject*> childrens;
};

