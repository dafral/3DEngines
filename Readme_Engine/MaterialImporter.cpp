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
	//delete the non erased materials
	for (std::list<Component_Material*>::iterator m = materials.begin(); m != materials.end(); )
	{
		RELEASE(*m);
		m = materials.erase(m);
	}

	return ret;
}

Component_Material* MaterialImporter::Import(const char* path)
{
	uint id = 0;
	ILenum error;
	Component_Material* m = nullptr;

	//Data will be handled by renderer. Devil is only used to load the image.
	if (ilLoad(IL_TYPE_UNKNOWN, path))
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

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &buff_id);
		glBindTexture(GL_TEXTURE_2D, buff_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), w, h, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

		id = buff_id;

		m = new Component_Material();
		materials.push_back(m);
		m->id_texture = id;
		m->width = w;
		m->height = h;
		m->path = path;

		SaveAsDDS();

		ilDeleteImage(ilGetInteger(IL_ACTIVE_IMAGE));
	}
	else
	{
		error = ilGetError();
		CONSOLELOG("Error loading image %s. Error %d.", path, error);
	}

	return m;
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

void MaterialImporter::RemoveMaterial(Component_Material * mat)
{
	for (std::list<Component_Material*>::iterator m = materials.begin(); m != materials.end(); ++m)
	{
		if (*m == mat)
		{
			RELEASE(*m);

			materials.erase(m);
			break;
		}
	}
}







