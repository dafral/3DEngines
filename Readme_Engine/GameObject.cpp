#include "Application.h"
#include "ModuleImgui.h"
#include "PanelHierarchy.h"
#include "Component.h"

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

	for (int i = 0; i < components.size(); i++)
		components[i]->Update();
}

void GameObject::Draw()
{
	Component_Material* material = (Component_Material*)FindComponent(COMPONENT_MATERIAL);
	Component_Transform* transform = (Component_Transform*)FindComponent(COMPONENT_TRANSFORM);

	glPushMatrix();
	if (transform != nullptr) glMultMatrixf(GetGlobalTransform(transform).ptr());

	for (int i = 0; i < components.size(); i++)
	{	
		if (components[i]->type == COMPONENT_MESH)
		{
			Component_Mesh* mesh = (Component_Mesh*)components[i];

			glEnableClientState(GL_VERTEX_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
			glVertexPointer(3, GL_FLOAT, 0, NULL);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);

			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_uvs);
			glTexCoordPointer(3, GL_FLOAT, 0, NULL);

			if (material != nullptr) glBindTexture(GL_TEXTURE_2D, (GLuint)material->id_texture);
			glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			glBindTexture(GL_TEXTURE_2D, 0);
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}
	}

	glPopMatrix();
}

void GameObject::CreateHierarchy()
{
	uint flag = 0; // Deafult

	// Setting flags
	if (childrens.empty())
		flag |= ImGuiTreeNodeFlags_Leaf;

	if (App->imgui->hierarchy->selected == this)
		flag |= ImGuiTreeNodeFlags_Selected;

	// Creating tree
	if (ImGui::TreeNodeEx(name.c_str(), flag))
	{
		// Know if a GO is selected
		if (ImGui::IsItemClicked())
			App->imgui->hierarchy->selected = this;

		if (ImGui::IsMouseDoubleClicked(0) && !ImGui::IsWindowFocused())
			App->imgui->hierarchy->selected = nullptr;

		// Recursion
		for (int i = 0; i < childrens.size(); i++)
			childrens[i]->CreateHierarchy();

		ImGui::TreePop();
	}
}

const Component* GameObject::FindComponent(component_type type)
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->type == type) return components[i];
	}
	return nullptr;
}

// This function deletes all components of that type
void GameObject::DeleteComponentType(component_type type)
{
	for (int i = 0; i < components.size(); )
	{
		if (components[i]->type == type)
		{
			delete components[i];
			components.erase(components.begin() + i);
		}
		else i++;
	}
}

// ============================================================================

const int GameObject::GetVertices()
{
	int t_vertices = 0;

	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->type == COMPONENT_MESH)
		{
			Component_Mesh* aux = (Component_Mesh*)components[i];
			t_vertices += aux->num_vertices;
		}
	}
	return t_vertices;
}

const int GameObject::GetIndices()
{
	int t_indices = 0;

	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->type == COMPONENT_MESH)
		{
			Component_Mesh* aux = (Component_Mesh*)components[i];
			t_indices += aux->num_indices;
		}
	}

	return t_indices;
}

const int GameObject::GetTextureWidth()
{
	Component_Material* aux = (Component_Material*)FindComponent(COMPONENT_MATERIAL);
	return aux->width;
}

const int GameObject::GetTextureHeight()
{
	Component_Material* aux = (Component_Material*)FindComponent(COMPONENT_MATERIAL);
	return aux->height;
}

void GameObject::SetTextureDimensions(int w, int h)
{
	Component_Material* aux = (Component_Material*)FindComponent(COMPONENT_MATERIAL);
	aux->width = w;
	aux->height = h;
}

const float4x4 GameObject::GetGlobalTransform(Component_Transform* trans)
{
	float4x4 ret = trans->GetTransform();

	if (parent != nullptr)
	{
		Component_Transform* parent_trans = (Component_Transform*)parent->FindComponent(COMPONENT_TRANSFORM);

		if (parent_trans != nullptr) ret = ret * parent_trans->GetTransform();
	}

	return ret;
}