#include "Skybox.h"

Skybox::Skybox()
{
	skyShader = nullptr;
	skyMesh = nullptr;

	textureID = 0;
	uniformProjection = 0;
	uniformView = 0;
}

Skybox::Skybox(std::vector<std::string> faceLoactions)
{
	// Shader setup
	skyShader = new Shader();
	skyShader->CreateShader("src/shaders/skyBox.vert", "src/shaders/skyBox.frag");

	uniformProjection = skyShader->GetProjectionLocation();
	uniformView = skyShader->GetViewLocation();

	// Texture setup
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, bitDepth;
	for (int i = 0; i < faceLoactions.size(); i++) {
		unsigned char* texData = stbi_load(faceLoactions[i].c_str(), &width, &height, &bitDepth, 0);
		if (!texData) {
			std::cout << "Failed to load skybox! : " << faceLoactions[i].c_str();
			stbi_image_free(texData);
			return;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
		stbi_image_free(texData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Mesh Setup

	float skyboxVertices[] = {
	   -1.0f,  1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   -1.0f, -1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f,  1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,

	   -1.0f,  1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f,  1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   -1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	};


	unsigned int skyboxIndices[] = {
		// Front
		0, 1, 2,
		2, 1, 3,

		// Right
		2, 3, 5,
		5, 3, 7,

		// Back
		5, 7, 4,
		4, 7, 6,

		// Left
		4, 6, 0,
		0, 6, 1,

		// Top
		4, 0, 5,
		5, 0, 2,

		// Bottom
		1, 6, 3,
		3, 6, 7
	};

	skyMesh = new Mesh();
	skyMesh->CreateMesh(skyboxVertices, skyboxIndices, 64, 36);
}


void Skybox::DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	viewMatrix = glm::mat4(glm::mat3(viewMatrix));

	glDepthMask(GL_FALSE);

	skyShader->UseShader();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	
	skyMesh->RenderMesh();

	glDepthMask(GL_TRUE);
}

Skybox::~Skybox()
{
	delete skyShader;
	delete skyMesh;
}