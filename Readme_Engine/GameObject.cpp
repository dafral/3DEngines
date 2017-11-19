#include "Application.h"
#include "ModuleImgui.h"
#include "PanelHierarchy.h"
#include "Component.h"

#include "GameObject.h"
#include "glew/include/GL/glew.h"
#include "Imgui/imgui.h"

GameObject::GameObject(std::string name, GameObject* parent) : name(name), parent(parent)
{
	LCG random;
	unique_id = random.Int(0, 2147483647);
}

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

void GameObject::OnSave(JSON_Doc* config)
{
	//config->MoveToSectionFromArray("GameObjects", config->GetArraySize("GameObjects") - 1);
	
	string aux = string("Gameobjects.") + name + string(".UID");
	config->SetNumber(aux.c_str(), unique_id);

	aux = string("Gameobjects.") + name + string(".Name");
	config->SetString(aux.c_str(), name.c_str());

	aux = string("Gameobjects.") + name + string(".Visible");
	config->SetBool(aux.c_str(), is_visible);

	aux = string("Gameobjects.") + name + string(".Static");
	config->SetBool(aux.c_str(), is_static);

	if (parent != nullptr) {

		aux = string("Gameobjects.") + name + string(".Parent");
		config->SetNumber(aux.c_str(), parent->unique_id);
	}
	else {
		aux = string("Gameobjects.") + name + string(".Parent");
		config->SetNumber(aux.c_str(), 0);

	}

	//Travel components
	/*for (int i = 0; i < components.size(); i++) {
		components[i]->OnSave(config);
	}*/

	//Travel Childs
	for (int i = 0; i < childrens.size(); i++) {
		childrens[i]->OnSave(config);
	}

}

void GameObject::OnLoad(JSON_Doc * config)
{

	string aux = string("Gameobjects.") + name + string(".UID");
	unique_id = config->GetNumber(aux.c_str());

	aux = string("Gameobjects.") + name + string(".Visible");
	is_visible = config->GetBool(aux.c_str());

	aux = string("Gameobjects.") + name + string(".Static");
	is_static = config->GetBool(aux.c_str());

	if (parent != nullptr) {

		aux = string("Gameobjects.") + name + string(".Parent");
		parent->unique_id = config->GetNumber(aux.c_str());
	}
	else {
		if (name.c_str() == "Root") {
			aux = string("Gameobjects.") + name + string(".Parent");
			parent->unique_id = 0;
		}

	}

	for (int i = 0; i < childrens.size(); i++) {
		childrens[i]->OnLoad(config);
	}
}