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
	if (!is_static) App->renderer3D->AddObjectToDraw(this);

	// Calling childrens Update()
	if (!childrens.empty())
	{
		for (int i = 0; i < childrens.size(); i++)
		{
			childrens[i]->Update();
		}
	}

	// Calling components Update()
	for (int i = 0; i < components.size(); i++)
		components[i]->Update();
}

void GameObject::Draw()
{
	Component_Material* material = (Component_Material*)FindComponent(COMPONENT_MATERIAL);
	Component_Transform* transform = (Component_Transform*)FindComponent(COMPONENT_TRANSFORM);
	Component_Mesh* mesh = (Component_Mesh*)FindComponent(COMPONENT_MESH);

	glPushMatrix();
	if (transform != nullptr) glMultMatrixf(GetGlobalTransform(transform).ptr());

	if (mesh != nullptr)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_uvs);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);

		if (material != nullptr) glBindTexture(GL_TEXTURE_2D, (GLuint)material->id_texture);
		glColor3f(1.0f, 1.0f, 1.0f);
		glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	glPopMatrix();

	// Bounding box
	if (mesh != nullptr) mesh->AdaptBoundingBox(this, transform);
}

void GameObject::CreateHierarchy()
{
	uint flag = 0; // Deafult

	// Setting flags
	if (childrens.empty())
		flag |= ImGuiTreeNodeFlags_Leaf;

	if (App->imgui->hierarchy->go_selected == this)
		flag |= ImGuiTreeNodeFlags_Selected;

	// Creating tree
	if (ImGui::TreeNodeEx(name.c_str(), flag))
	{
		// Know if a GO is selected
		if (ImGui::IsItemClicked())
			App->imgui->hierarchy->go_selected = this;

		if (ImGui::IsMouseDoubleClicked(1) && !ImGui::IsWindowFocused())
			App->imgui->hierarchy->go_selected = nullptr;

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
	float4x4 ret = trans->GetTransform().Transposed();

	if (parent != nullptr)
	{
		Component_Transform* parent_trans = (Component_Transform*)parent->FindComponent(COMPONENT_TRANSFORM);

		if (parent_trans != nullptr) ret = ret * parent_trans->GetTransform().Transposed();
	}

	return ret;
}

// ----------------------------------------------------------

void GameObject::SetVisible(bool new_visible)
{ 
	is_visible = new_visible;

	// Doing the same for childs
	for (int i = 0; i < childrens.size(); i++)
		childrens[i]->SetVisible(new_visible);
};

void GameObject::SetStatic(bool new_static) 
{ 
	is_static = new_static; 

	// Adding to or removing from static list
	if (is_static) App->octree->AddStatic(this);
	else App->octree->RemoveStatic(this);

	// Doing the same for childs
	for (int i = 0; i < childrens.size(); i++)
		childrens[i]->SetStatic(new_static);
};

//-------------------------------------------------------

void GameObject::OnSave(JSON_Doc& config)
{
	Component_Transform* trans = (Component_Transform*)FindComponent(COMPONENT_TRANSFORM);
	Component_Mesh* mesh = (Component_Mesh*)FindComponent(COMPONENT_MESH);
	Component_Camera* cam = (Component_Camera*)FindComponent(COMPONENT_CAMERA);
	Component_Material* material = (Component_Material*)FindComponent(COMPONENT_MATERIAL);

	//if (trans)
		//trans->OnSave(*App->json->config);

	/*if (mesh)
		mesh->OnSave(*App->json->config);*/

	/*if (material)
		material->OnSave(*App->json->config);*/

	//if (cam)
		//cam->OnSave(*App->json->config);

}

void GameObject::OnLoad(JSON_Doc * config)
{

}