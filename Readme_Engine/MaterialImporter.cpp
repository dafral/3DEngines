#include "Application.h"
#include "MaterialImporter.h"
#include "ModuleGeometry.h"
#include "ModuleScene.h"
#include "PanelProperties.h"
#include "Component.h"
#include "GameObject.h"
#include <string>

#include "glew/include/GL/glew.h"
#include "MathGeoLib/MathGeoLib.h"

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

MaterialImporter::MaterialImporter(Application * app, bool start_enabled) : Module(app, start_enabled)
{}

MaterialImporter::~MaterialImporter()
{}

bool MaterialImporter::Init()
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);

	return true;
}

bool MaterialImporter::CleanUp()
{
	bool ret = true;

	return ret;
}

void MaterialImporter::Import(const char* full_path, GameObject* go)
{
	ILenum error;
	Component_Material* aux = IsMaterialLoaded(full_path);

	//Data will be handled by renderer. Devil is only used to load the image.
	if (aux == nullptr && ilLoad(IL_TYPE_UNKNOWN, full_path))
	{
		ILinfo info;
		iluGetImageInfo(&info);
		if (info.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE);
		int w = ilGetInteger(IL_IMAGE_WIDTH);
		int h = ilGetInteger(IL_IMAGE_HEIGHT);
		uint buff_id = 0;

		glEnable(GL_TEXTURE_2D);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &buff_id);
		glBindTexture(GL_TEXTURE_2D, buff_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), w, h, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

		if (go->FindComponent(COMPONENT_MATERIAL) != nullptr) go->DeleteComponentType(COMPONENT_MATERIAL);
		Component_Material* new_component = new Component_Material();
		go->AddComponent(new_component);

		new_component->id_texture = buff_id;
		new_component->width = w;
		new_component->height = h;
		new_component->path = full_path;

		paths_loaded.push_back(new_component);

		SaveAsDDS();

		ilDeleteImage(ilGetInteger(IL_ACTIVE_IMAGE));
	}
	else if (aux != nullptr)
	{
		if (go->FindComponent(COMPONENT_MATERIAL) != nullptr) go->DeleteComponentType(COMPONENT_MATERIAL);
		Component_Material* new_component = new Component_Material();
		go->AddComponent(new_component);

		new_component->id_texture = aux->id_texture;
		new_component->width = aux->width;
		new_component->height = aux->height;
		new_component->path = aux->path;
	}
	else
	{
		error = ilGetError();
		CONSOLELOG("Error loading image %s. Error %d.", full_path, error);
	}
}

void MaterialImporter::SaveAsDDS()
{
	ILuint		size;
	ILubyte*	data;

	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);

	size = ilSaveL(IL_DDS, NULL, 0);
	if (size > 0)
	{
		data = new ILubyte[size];
		if (ilSaveL(IL_DDS, data, size) > 0)
		{
			char file[69];
			sprintf_s(file, "Library\\Materials\\texture_%d.dds", save_id++);
			FILE* tex_file = fopen(file, "wb");
			fwrite(data, sizeof(ILubyte), size, tex_file);
			fclose(tex_file);
			CONSOLELOG("New material texture saved: %s.", file);
		}
	}
}

Component_Material* MaterialImporter::IsMaterialLoaded(string path)
{
	Component_Material* ret = nullptr;

	for (int i = 0; i < paths_loaded.size(); i++)
	{
		if (paths_loaded[i]->path == path)
			ret = paths_loaded[i];
	}

	return ret;
}








