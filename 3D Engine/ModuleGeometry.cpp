#include "Application.h"
#include "ModuleGeometry.h"
#include "PanelProperties.h"
#include "Component.h"
#include "GameObject.h"

#include "glew/include/GL/glew.h"
#include "src/MathGeoLib.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")
#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

ModuleGeometry::ModuleGeometry(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

ModuleGeometry::~ModuleGeometry()
{}

bool ModuleGeometry::Init()
{
	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	// DeviL Initialization
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	return true;
}

update_status ModuleGeometry::Update(float dt)
{
	return UPDATE_CONTINUE;
}                                                                           

bool ModuleGeometry::CleanUp()
{
	// detach log stream
	aiDetachAllLogStreams();
	return true;
}

void ModuleGeometry::LoadMeshes(const char* full_path, GameObject* go)
{
	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		if (go->FindComponent(COMPONENT_MESH) != nullptr) go->DeleteComponentType(COMPONENT_MESH);

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			CONSOLELOG("Creating component mesh for Game Object '%s'", go->name.c_str());
		
			Component_Mesh* new_component = new Component_Mesh;
			go->AddComponent(new_component);

			// Vertices
			aiMesh* new_mesh = scene->mMeshes[i];
			new_component->num_vertices = new_mesh->mNumVertices;
			new_component->vertices = new float[new_component->num_vertices * 3];
			memcpy(new_component->vertices, new_mesh->mVertices, sizeof(float) * new_component->num_vertices * 3);

			// Load buffer for vertices
			glGenBuffers(1, (GLuint*) &(new_component->id_vertices));
			glBindBuffer(GL_ARRAY_BUFFER, new_component->id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*new_component->num_vertices * 3, new_component->vertices, GL_STATIC_DRAW);

			// Indices
			if (new_mesh->HasFaces())
			{
				new_component->num_indices = new_mesh->mNumFaces * 3;
				new_component->indices = new uint[new_component->num_indices];
				for (uint i = 0; i < new_mesh->mNumFaces; i++)
				{
					if (new_mesh->mFaces[i].mNumIndices != 3) {
						CONSOLELOG("WARNING, geometry face with != 3 indices!");
					}
					else {
						memcpy(&new_component->indices[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}

				// Load buffer for indices
				glGenBuffers(1, (GLuint*) &(new_component->id_indices));
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_component->id_indices);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * new_component->num_indices, new_component->indices, GL_STATIC_DRAW);
			}

			// UVs
			if (new_mesh->HasTextureCoords(0))
			{
				new_component->num_uvs = new_mesh->mNumVertices;
				new_component->texture_coords = new float[new_component->num_uvs * 3];
				memcpy(new_component->texture_coords, new_mesh->mTextureCoords[0], sizeof(float) * new_component->num_uvs * 3);

				// Load buffer for UVs
				glGenBuffers(1, (GLuint*) &(new_component->id_uvs));
				glBindBuffer(GL_ARRAY_BUFFER, (GLuint)new_component->id_uvs);
				glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * new_component->num_uvs * 3, new_component->texture_coords, GL_STATIC_DRAW);
			}		

			// Focusing camera to the FBX 
			AABB box;
			box.SetNegativeInfinity();
			box.Enclose((float3*)new_mesh->mVertices, new_mesh->mNumVertices);

			vec3 midpoint = (box.CenterPoint().x, box.CenterPoint().y, box.CenterPoint().z);
			App->imgui->properties->SetPosition(midpoint);
			App->camera->Position = midpoint + (App->camera->Z *  box.Size().Length() * 1.2f);
		}

		aiReleaseImport(scene);
	}
	else {
		CONSOLELOG("Error loading scene %s", full_path);
	}
}

void ModuleGeometry::LoadMaterial(const char* full_path, GameObject* go)
{
	App->imgui->properties->GetTextureName();

	ILuint imageID;
	ILenum error;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	if (ilLoadImage(full_path))
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		CONSOLELOG("Creating component material for Game Object '%s'", go->name.c_str());

		if (go->FindComponent(COMPONENT_MATERIAL) != nullptr) go->DeleteComponentType(COMPONENT_MATERIAL);
		Component_Material* new_component = new Component_Material();
		go->AddComponent(new_component);

		// Flip the image if it is upside-down
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
			iluFlipImage();

		if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE))
		{
			error = ilGetError();
			CONSOLELOG("Texture conversion failed: %d %s", error, iluErrorString(error));
		}
		else 
		{
			// Enabling texture
			glEnable(GL_TEXTURE_2D);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &new_component->id_texture);
			glBindTexture(GL_TEXTURE_2D, new_component->id_texture);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			// Texture specifications
			SetTextureDimensions(ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
			//glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), new_component->width, new_component->height, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
			glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());
		}
	}
	else {
		error = ilGetError();
	}

	ilDeleteImages(1, &imageID);
}

const int ModuleGeometry::GetVertices()
{
	/*int t_vertices = 0;

	for (int i = 0; i < meshes.size(); i++)
		t_vertices += meshes[i]->num_vertices; 

	return t_vertices;*/
	return 0;
}

const int ModuleGeometry::GetIndices()
{
	//int t_indices = 0;

	//for (int i = 0; i < meshes.size(); i++)
	//	t_indices += meshes[i]->num_indices;

	/*return t_indices;*/

	return 0;
}

const int ModuleGeometry::GetTextureWidth()
{
	//return tex.width;
	return 0;
}

const int ModuleGeometry::GetTextureHeight()
{
	//return tex.height;
	return 0;
}

void ModuleGeometry::SetTextureDimensions(int w, int h)
{
	//tex.width = w;
	//tex.height = h;
}
