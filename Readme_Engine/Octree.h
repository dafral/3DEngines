#pragma once
#include <vector>
#include "GameObject.h"

class Octree
{
public:
	Octree() {};
	~Octree() {};

	void AddStatic(GameObject* static_go);
	void RemoveStatic(GameObject* static_go);
	void StartOctree();
	void SplitOctree();

private:
	std::vector<GameObject*> all_static_go;
};