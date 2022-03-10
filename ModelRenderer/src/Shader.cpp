#include "Shader.h"

Shader::Shader() {
	pointLightCount = 0;
	spotLightCount = 0;

	uniformPointLightCount = 0;
	uniformSpotLightCount = 0;

	shaderID = 0;
	uniformModel = 0;
	uniformProjection = 0;
	uniformView = 0;
	vertex = 0;
	fragment = 0;

	uniformSpecularIntensity = 0;
	uniformShininess = 0;
	uniformEyePosition = 0;

	uniformDirectionalLightTransform = 0;
	uniformDirectionalShadowMap = 0;
	uniformTexture = 0;
}

std::string Shader::ReadShader(const char* filePath) {
	std::stringstream buffer;
	buffer << std::ifstream(filePath).rdbuf();
	return buffer.str();
}

void Shader::CreateShader(const char* vertexPath, const char* fragmentPath) {
	std::string vertexSource = ReadShader(vertexPath);
	std::string fragmentSource = ReadShader(fragmentPath);

	const char* vertexCode = vertexSource.c_str();
	const char* fragmentCode = fragmentSource.c_str();

	CompileShader(vertexCode, fragmentCode);
}


unsigned int Shader::GetModelLocation() {
	return uniformModel;
}

unsigned int Shader::GetProjectionLocation() {
	return uniformProjection;
}

unsigned int Shader::GetViewLocation() {
	return uniformView;
}

unsigned int Shader::GetAmbientColorLocation() {
	return uniformDirectionalLight.uniformColor;
}

unsigned int Shader::GetAmbientIntensityLocation() {
	return uniformDirectionalLight.uniformAmbientIntensity;
}


unsigned int Shader::GetDiffuseIntensityLocation() {
	return uniformDirectionalLight.uniformDiffuseIntensity;
}

unsigned int Shader::GetDirectionLocation() {
	return uniformDirectionalLight.uniformDirection;
}

unsigned int Shader::GetSpecularIntensityLocation() {
	return uniformSpecularIntensity;
}

unsigned int Shader::GetShininessLocation() {
	return uniformShininess;
}

unsigned int Shader::GetEyeLocation() {
	return uniformEyePosition;
}

void Shader::SetDirectionalLight(DirectionalLight* dLight)
{
	dLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColor,
		uniformDirectionalLight.uniformDiffuseIntensity, uniformDirectionalLight.uniformDirection);
}

void Shader::SetPointLight(PointLight* pLight, unsigned int lightCount) {
	if (lightCount > MAX_POINT_LIGHTS) {
		lightCount = MAX_POINT_LIGHTS;
	}

	glUniform1i(uniformPointLightCount, lightCount);

	for (int i = 0; i < lightCount; i++) {
		pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColor,
						   uniformPointLight[i].uniformDiffuseIntensity, uniformPointLight[i].uniformPosition,
						   uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
	}
}

void Shader::SetSpotLight(SpotLight* sLight, unsigned int lightCount)
{
	if (lightCount > MAX_SPOT_LIGHTS) {
		lightCount = MAX_SPOT_LIGHTS;
	}

	glUniform1i(uniformSpotLightCount, lightCount);

	for (int i = 0; i < lightCount; i++) {
		sLight[i].UseLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColor, 
			uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformPosition, 
			uniformSpotLight[i].uniformDirection, uniformSpotLight[i].uniformEdge, 
			uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent);
	}
}

void Shader::AddShader(unsigned int program, const char* shaderSource, int shaderType) {

	if (shaderType == GL_VERTEX_SHADER) {
		vertex = glCreateShader(shaderType);
		glShaderSource(vertex, 1, &shaderSource, nullptr);
		glCompileShader(vertex);
	}

	if (shaderType == GL_FRAGMENT_SHADER) {
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &shaderSource, nullptr);
		glCompileShader(fragment);
	}

	glAttachShader(program, vertex);
	glAttachShader(program, fragment);
}

void Shader::CompileShader(const char* vertexSource, const char* fragmentSource) {

	shaderID = glCreateProgram();

	AddShader(shaderID, vertexSource, GL_VERTEX_SHADER);
	AddShader(shaderID, fragmentSource, GL_FRAGMENT_SHADER);

	glLinkProgram(shaderID);
	glValidateProgram(shaderID);

	// Unifroms
	uniformProjection = glGetUniformLocation(shaderID, "projection");
	uniformModel = glGetUniformLocation(shaderID, "model");
	uniformView = glGetUniformLocation(shaderID, "view");

	uniformDirectionalLight.uniformColor = glGetUniformLocation(shaderID, "directionalLight.base.color");
	uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");

	uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
	uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
	uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");

	uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");
	for (int i = 0; i < MAX_POINT_LIGHTS; i++) {
		char buffer[100] = { '\0' };

		snprintf(buffer, sizeof(buffer), "pointLight[%d].base.color", i);
		uniformPointLight[i].uniformColor = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "pointLight[%d].base.ambientIntensity", i);
		uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "pointLight[%d].base.diffuseIntensity", i);
		uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "pointLight[%d].position", i);
		uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "pointLight[%d].constant", i);
		uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "pointLight[%d].linear", i);
		uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "pointLight[%d].exponent", i);
		uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, buffer);
	}


	// Spot Light

	uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");
	for (int i = 0; i < MAX_SPOT_LIGHTS; i++) {
		char buffer[100] = { '\0' };

		snprintf(buffer, sizeof(buffer), "spotLight[%d].base.base.color", i);
		uniformSpotLight[i].uniformColor = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "spotLight[%d].base.base.ambientIntensity", i);
		uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "spotLight[%d].base.base.diffuseIntensity", i);
		uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "spotLight[%d].base.position", i);
		uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "spotLight[%d].base.constant", i);
		uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "spotLight[%d].base.linear", i);
		uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "spotLight[%d].base.exponent", i);
		uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "spotLight[%d].direction", i);
		uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, buffer);

		snprintf(buffer, sizeof(buffer), "spotLight[%d].edge", i);
		uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, buffer);
	}

	uniformTexture = glGetUniformLocation(shaderID, "theTexture");
	uniformDirectionalLightTransform = glGetUniformLocation(shaderID, "directionalLightTransform");
	uniformDirectionalShadowMap = glGetUniformLocation(shaderID, "directionalShadowMap");
}

void Shader::SetTexture(unsigned int textureUnit)
{
	glUniform1i(uniformTexture, textureUnit);
}

void Shader::SetDirectionalShadowMap(unsigned int textureUnit)
{
	glUniform1i(uniformDirectionalShadowMap, textureUnit);
}

void Shader::SetDirectionalLightTransform(glm::mat4* lightTransform)
{
	glUniformMatrix4fv(uniformDirectionalLightTransform, 1, GL_FALSE, glm::value_ptr(*lightTransform));
}

void Shader::UseShader() {
	glUseProgram(shaderID);
}

void Shader::EndShader() {
	glUseProgram(0);
}

void Shader::ClearShader() {
	if (shaderID != 0) {
		glDeleteProgram(shaderID);
		shaderID = 0;
	}

	if (vertex != 0) {
		glDeleteShader(vertex);
		vertex = 0;
	}

	if (fragment != 0) {
		glDeleteShader(fragment);
		fragment = 0;
	}

	uniformModel = 0;
}


Shader::~Shader() {
	ClearShader();
}