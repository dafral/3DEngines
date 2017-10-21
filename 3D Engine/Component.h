#pragma once

enum component_type {COMPONENT_MESH, COMPONENT_TRANSFORM, COMPONENT_MATERIAL};

class Component
{
public:
	virtual void Enable() {};
	virtual void Disable() {};
	virtual void Update() {};

public:
	component_type type;
};

class Component_Mesh : public Component
{
public:

};

class Component_Material : public Component
{
public:

};
