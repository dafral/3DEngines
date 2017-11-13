#include "Octree.h"

void Octree::AddStatic(GameObject* static_go)
{
	all_static_go.push_back(static_go);
}

void Octree::RemoveStatic(GameObject* static_go)
{
	for (int i = 0; i < all_static_go.size(); i++)
	{
		if (all_static_go[i] == static_go)
		{
			all_static_go.erase(all_static_go.begin() + i);
		}
	}
}

void Octree::StartOctree()
{

}

void Octree::SplitOctree()
{

}