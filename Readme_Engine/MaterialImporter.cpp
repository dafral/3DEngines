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

bool MaterialImporter::Import(const char* file, const char* path, std::string& output_file) 
{
	bool ret = false;

	ILuint size;
	ILubyte *data;

	ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);// To pick a specific DXT compression use
	size = ilSaveL(IL_DDS, NULL, 0); // Get the size of the data buffer

	if (size > 0) 
	{
		data = new ILubyte[size]; // allocate data buffer
		if (ilSaveL(IL_DDS, data, size) > 0) // Save to buffer with the ilSaveIL function
			ret = App->filesystem->SaveUnique("Library/Textures/", (char*)data, file, "dds", size, output_file, (file == ""));
		
		if (data != nullptr) {
			delete[] data;
		}
	}

	if (ret == false)
		CONSOLELOG("ERROR importing texture");

	return ret;
}

void MaterialImporter::Load(const char* full_path, GameObject* go)
{
	ILuint imageID;
	ILenum error;

	ilGenImages(1, &imageID);
	ilBindImage(imageID);

	if (ilLoadImage(full_path))
	{
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

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
			go->SetTextureDimensions(ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
			glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), new_component->width, new_component->height, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, ilGetData());

			CONSOLELOG("Texture loaded!");
		}
	}
	else {
		error = ilGetError();
	}

	ilDeleteImages(1, &imageID);
}





