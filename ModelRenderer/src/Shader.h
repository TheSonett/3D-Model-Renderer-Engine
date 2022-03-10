#pragma once

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"


#define MAX_POINT_LIGHTS 3
#define MAX_SPOT_LIGHTS 3

class Shader {
public:
	Shader();
	~Shader();
public:
	void CreateShader(const char* vertexPath, const char* fragmentPath);
	void UseShader();
	void ClearShader();
	void EndShader();

	unsigned int GetModelLocation();
	unsigned int GetProjectionLocation();
	unsigned int GetViewLocation();
	unsigned int GetAmbientColorLocation();
	unsigned int GetAmbientIntensityLocation();
	unsigned int GetDiffuseIntensityLocation();
	unsigned int GetDirectionLocation();
	unsigned int GetSpecularIntensityLocation();
	unsigned int GetShininessLocation();
	unsigned int GetEyeLocation();

	void SetDirectionalLight(DirectionalLight* dLight);
	void SetPointLight(PointLight* pLight, unsigned int lightCount);
	void SetSpotLight(SpotLight* sLight, unsigned int lightCount);
	void SetTexture(unsigned int textureUnit);
	void SetDirectionalShadowMap(unsigned int textureUnit);
	void SetDirectionalLightTransform(glm::mat4* lightTransform);

private:
	int pointLightCount;
	int spotLightCount;

	unsigned int shaderID;
	unsigned int vertex, fragment;
	unsigned int uniformModel, uniformProjection, uniformView;
	unsigned int uniformEyePosition, uniformSpecularIntensity, uniformShininess;
	unsigned int uniformDirectionalLightTransform, uniformDirectionalShadowMap, uniformTexture;

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	} uniformDirectionalLight;

	GLint uniformPointLightCount;

	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	} uniformPointLight[MAX_POINT_LIGHTS];

	GLint uniformSpotLightCount;
	struct {
		GLuint uniformColor;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLint uniformDirection;
		GLint uniformEdge;
	} uniformSpotLight[MAX_SPOT_LIGHTS];

	std::string ReadShader(const char* filePath);
	void CompileShader(const char* vertexSource, const char* fragmentSource);
	void AddShader(unsigned int program, const char* shaderSource, int shaderType);
};