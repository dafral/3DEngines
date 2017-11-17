#pragma once
#include <vector>

class GameObject;
class Component_Camera;

#define MAX_OBJ_IN_NODE 6
#define MAX_DIVISIONS 100
#define SUBDIVISIONS 8

class Octree_Node
{
public:
	Octree_Node(AABB box) : box(box) {};
	~Octree_Node() {};

	void Draw();
	void CleanUp();
	void CollectFrustumIntersections(Component_Camera* curr_camera);

	void DivideNode();
	const bool IsFull() { return objects_in_node.size() > MAX_OBJ_IN_NODE; };

private:
	AABB box;
	bool divided = false;
	uint divisions = 0;
	Octree_Node* childs[SUBDIVISIONS];

public:
	std::vector<GameObject*> objects_in_node;
};

class Octree
{
public:
	Octree() { root_node = nullptr; };
	~Octree() {};

	void AddStatic(GameObject* static_go);
	void RemoveStatic(GameObject* static_go);
	void StartOctree();

	void Draw();
	void CleanUp();
	void CollectFrustumIntersections(Component_Camera* curr_camera);

private:
	Octree_Node* root_node = nullptr;
	std::vector<GameObject*> all_static_go;	

public:
	uint divisions = 0;
};