#include "Application.h"
#include "ModuleImgui.h"
#include "PanelInspector.h"

#include "GameObject.h"
#include "glew/include/GL/glew.h"
#include "Imgui/imgui.h"

GameObject::GameObject(std::string name, GameObject* parent) : name(name), parent(parent)
{}

GameObject::~GameObject()
{}

void GameObject::AddChildren(GameObject* children)
{
	childrens.push_back(children);
}

void GameObject::AddComponent(Component* component)
{
	components.push_back(component);
}

void GameObject::Update()
{
	// Doing stuff
	Draw();

	// Calling childrens Update()
	if (!childrens.empty())
	{
		for (int i = 0; i < childrens.size(); i++)
		{
			childrens[i]->Update();
		}
	}
}

void GameObject::Draw()
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->type == COMPONENT_MESH)
		{
			Component_Mesh* mesh = (Component_Mesh*)components[i];

			glEnableClientState(GL_VERTEX_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
			glVertexPointer(3, GL_FLOAT, 0, NULL);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
			glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_uvs);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			glDisableClientState(GL_VERTEX_ARRAY);
		}

		if (components[i]->type == COMPONENT_MATERIAL)
		{
			Component_Material* material = (Component_Material*)components[i];

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(3, GL_FLOAT, 0, NULL);
			glBindTexture(GL_TEXTURE_2D, material->id_texture);
			glBindTexture(GL_TEXTURE_2D, 0);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
	}
}

void GameObject::CreateHierarchy()
{
	uint flag = 0; // Deafult

	// Setting flags
	if (childrens.empty())
		flag |= ImGuiTreeNodeFlags_Leaf;

	if (App->imgui->inspector->selected == this) 
		flag |= ImGuiTreeNodeFlags_Selected;

	// Creating tree
	if (ImGui::TreeNodeEx(name.c_str(), flag))
	{
		// Know if a GO is selected
		if (ImGui::IsItemClicked())
			App->imgui->inspector->selected = this;

		// Recursion
		for (int i = 0; i < childrens.size(); i++)
			childrens[i]->CreateHierarchy();

		ImGui::TreePop();
	}
}