#pragma once
#include <vector>

class GameObject;

class Octree_Node
{
public:
	Octree_Node(AABB box) : box(box) {};
	~Octree_Node() {};

	void Draw();
	void CleanUp();
	void DivideNode();
	bool IsFull() { return objects_in_node.size() > 1; };

public:
	AABB box;

	bool divided = false;
	Octree_Node* childs[8];

	std::vector<GameObject*> objects_in_node;
};

class Octree
{
public:
	Octree() {};
	~Octree() {};

	void AddStatic(GameObject* static_go);
	void RemoveStatic(GameObject* static_go);
	void StartOctree();
	void Draw();
	void CleanUp();

private:
	std::vector<GameObject*> all_static_go;
	Octree_Node* root_node;
};