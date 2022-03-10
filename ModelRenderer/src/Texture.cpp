#include "Texture.h"

#include <iostream>

Texture::Texture(const char* filePath)
	: textureID(0), textureWidth(0), textureHeight(0), textureBitDepth(0)
{
	this->filePath = filePath;
}


bool Texture::LoadAlphaTexture()
{
	unsigned char* textureData = stbi_load(filePath, &textureWidth, &textureHeight, &textureBitDepth, 0);
	if (!textureData) {
		std::cout << "Failed to find texture : " << filePath << std::endl;
		return false;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(textureData);

	return true;
}

bool Texture::LoadTexture()
{
	unsigned char* textureData = stbi_load(filePath, &textureWidth, &textureHeight, &textureBitDepth, 0);
	if (!textureData) {
		std::cout << "Failed to find texture : " << filePath << std::endl;
		return false;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(textureData);

	return true;
}


void Texture::UseTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	textureWidth = 0;
	textureHeight = 0;
	textureBitDepth = 0;
	filePath = NULL;
}

Texture::~Texture()
{
	ClearTexture();
}