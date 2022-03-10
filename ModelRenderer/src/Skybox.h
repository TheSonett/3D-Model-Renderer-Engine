#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <stb_image/stb_image.h>
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"

class Skybox {
public:
	Skybox();
	Skybox(std::vector<std::string> faceLoactions);
	~Skybox();
public:
	void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
private:
	Mesh* skyMesh;
	Shader* skyShader;

	unsigned int textureID;
	unsigned int uniformProjection, uniformView;
};