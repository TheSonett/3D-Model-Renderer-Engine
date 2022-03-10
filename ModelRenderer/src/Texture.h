#pragma once

#include <GL/glew.h>
#include "vendor/stb_image/stb_image.h"

class Texture {
public:
	Texture() = default;
	Texture(const char* filePath);
	~Texture();
public:
	bool LoadTexture();
	bool LoadAlphaTexture();

	void UseTexture();
	void ClearTexture();
private:
	unsigned int textureID;
	int textureWidth, textureHeight, textureBitDepth;
	const char* filePath;
};