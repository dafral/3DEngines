#include "Application.h"
#include "ModuleGeometry.h"

#include "glew/include/GL/glew.h"

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

void ModuleGeometry::LoadGeometry(const char* full_path)
{
	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);
	CONSOLELOG("Loading scene with %d meshes.", scene->mNumMeshes);

	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			mesh_data data;

			// Vertices
			aiMesh* new_mesh = scene->mMeshes[i];
			data.num_vertices = new_mesh->mNumVertices;
			data.vertices = new float[data.num_vertices * 3];
			memcpy(data.vertices, new_mesh->mVertices, sizeof(float) * data.num_vertices * 3);
			CONSOLELOG("Mesh %d with %d vertices.", i+1, data.num_vertices);

			// Load buffer for vertices
			glGenBuffers(1, (GLuint*) &(data.id_vertices));
			glBindBuffer(GL_ARRAY_BUFFER, data.id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)*data.num_vertices * 3, data.vertices, GL_STATIC_DRAW);

			// Indices
			if (new_mesh->HasFaces())
			{
				data.num_indices = new_mesh->mNumFaces * 3;
				data.indices = new uint[data.num_indices];
				for (uint i = 0; i < new_mesh->mNumFaces; i++)
				{
					if (new_mesh->mFaces[i].mNumIndices != 3) {
						CONSOLELOG("WARNING, geometry face with != 3 indices!");
					}
					else {
						memcpy(&data.indices[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}

				// Load buffer for indices
				glGenBuffers(1, (GLuint*) &(data.id_indices));
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.id_indices);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * data.num_indices, data.indices, GL_STATIC_DRAW);
			}

			// UVs
			if (new_mesh->HasTextureCoords(0))
			{
				data.num_uvs = new_mesh->mNumVertices;
				data.texture_coords = new float[data.num_uvs * 3];
				memcpy(data.texture_coords, new_mesh->mTextureCoords[0], sizeof(float) * data.num_uvs * 3);

				// Load buffer for UVs
				glGenBuffers(1, (GLuint*) &(data.id_uvs));
				glBindBuffer(GL_ARRAY_BUFFER, (GLuint)data.id_uvs);
				glBufferData(GL_ARRAY_BUFFER, sizeof(uint) * data.num_uvs * 3, data.texture_coords, GL_STATIC_DRAW);
			}		
			meshes.push_back(data);
		}
		aiReleaseImport(scene);
	}
	else {
		CONSOLELOG("Error loading scene %s", full_path);
	}
}

void ModuleGeometry::LoadTexture(const char* full_path)
{
	ILuint imageID;
	ILenum error;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	if (ilLoadImage(full_path))
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		//Flip the image if it is upside-down
		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT) {
			iluFlipImage();
		}

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

			glGenTextures(1, &tex.id_texture);
			glBindTexture(GL_TEXTURE_2D, tex.id_texture);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			//Texture Specifications
			glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());	
		}
	}
	else {
		error = ilGetError();
	}

	ilDeleteImages(1, &imageID);
}

void ModuleGeometry::Draw()
{
	for (int i = 0; i < meshes.size(); i++)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, meshes[i].id_vertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshes[i].id_indices);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, meshes[i].id_uvs);
		glTexCoordPointer(3, GL_FLOAT, 0, NULL);

		glBindTexture(GL_TEXTURE_2D, (GLuint)tex.id_texture);
		glDrawElements(GL_TRIANGLES, meshes[i].num_indices, GL_UNSIGNED_INT, NULL);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

const int ModuleGeometry::GetVertices()
{
	int t_vertices = 0;

	for (int i = 0; i < meshes.size(); i++)
		t_vertices += meshes[i].num_vertices; 

	return t_vertices;
}

const int ModuleGeometry::GetIndices()
{
	int t_indices = 0;

	for (int i = 0; i < meshes.size(); i++)
		t_indices += meshes[i].num_indices;

	return t_indices;
}
