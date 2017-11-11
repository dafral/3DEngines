#include "Application.h"
#include "PanelProperties.h"
#include "ModuleGeometry.h"
#include "ModuleImgui.h"
#include "PanelHierarchy.h"
#include "GameObject.h"
#include "MathGeoLib/MathGeoLib.h"

PanelProperties::PanelProperties(bool active = true) : Panel(active)
{
	x = 15;
	y = 660;
	w = 223;
	h = 349;
}

PanelProperties::~PanelProperties()
{}

void PanelProperties::Draw()
{
	ImGui::SetNextWindowPos(ImVec2(x, y));
	ImGui::SetNextWindowContentSize(ImVec2(w, h));

	GameObject* go = App->imgui->hierarchy->selected;

	if (go != nullptr/* && go != App->scene->root*/)
	{
		ImGui::Begin("Properties", &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

		Component_Transform* trans = (Component_Transform*)go->FindComponent(COMPONENT_TRANSFORM);
		Component_Mesh* mesh = (Component_Mesh*)go->FindComponent(COMPONENT_MESH);
		Component_Camera* cam = (Component_Camera*)go->FindComponent(COMPONENT_CAMERA);
		Component_Material* material = (Component_Material*)go->FindComponent(COMPONENT_MATERIAL);

		// Name, static and visible
		ImGui::Text("Name: %s", go->name.c_str());

		if (ImGui::Checkbox("Static", &go->is_static))
			go->SetStatic(go->is_static);

		if (ImGui::Checkbox("Visible", &go->is_visible))
			go->SetVisible(go->is_visible);

		// Transformation
		if (trans != nullptr)
		{
			if (ImGui::CollapsingHeader("Transform"))
			{
				float3 position = trans->GetPosition();
				float3 scale = trans->GetScale();
				Quat rotation = trans->GetRotation();

				float3 euler_rotation = RadToDeg(rotation.ToEulerXYZ());

				ImGui::Text("  X       Y      Z");

				if (ImGui::DragFloat3("Position", (float*)&position, 0.1f))
					trans->SetPosition(position);

				if (ImGui::DragFloat3("Rotation", (float*)&euler_rotation, 0.1f))
					trans->SetRotation(DegToRad(euler_rotation));
					
				if (ImGui::DragFloat3("Scale", (float*)&scale, 1.0f))
					trans->SetScale(scale);	

				if (cam != nullptr) 
				{
					cam->SetPos(position);
					cam->SetZDir(trans->GetTransform().WorldZ());
					cam->SetYDir(trans->GetTransform().WorldY());
				}
			}
		}

		// Meshes
		if (mesh != nullptr)
		{
			if (ImGui::CollapsingHeader("Meshes"))
			{
				ImGui::Text("Vertices: %d", go->GetVertices());
				ImGui::Text("Triangles: %d", go->GetIndices() / 3);
			}
		}

		// Texture
		if (material != nullptr)
		{
			if (ImGui::CollapsingHeader("Texture"))
			{
				ImGui::Text("Texture name: %s", texture_name.c_str());
				ImGui::Text("Texture dimensions: %d x %d", go->GetTextureWidth(), go->GetTextureHeight());
			}
		}

		// Camera
		if (cam != nullptr)
		{
			if (ImGui::CollapsingHeader("Camera"))
			{
				if (ImGui::Checkbox("Active", &cam->active_camera))
				{
					cam->SwitchActive(cam->active_camera);
					App->scene->SetActiveCam(cam);
				}	
			}
		}

		ImGui::End();
	}
}

void PanelProperties::SetGeometryName(char* reference)
{
	std::string str = reference;

	int dot_pos = str.find_last_of(".");
	int bar_pos = str.find_last_of("\\");
	int name_length = dot_pos - bar_pos - 1;

	geometry_name = str.substr(bar_pos + 1, name_length);
}

const char* PanelProperties::GetGeometryName()
{
	return geometry_name.c_str();
}

void PanelProperties::SetTextureName(char* reference)
{
	std::string str = reference;

	int dot_pos = str.find_last_of(".");
	int bar_pos = str.find_last_of("\\");
	int name_length = dot_pos - bar_pos - 1;

	texture_name = str.substr(bar_pos + 1, name_length);
}
