#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	lightProj = glm::ortho(-5.0f, 5.0f, -5.f, 5.0f, 0.1f, 20.0f);
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
									GLfloat ambientIntensity, GLfloat diffuseIntensity,
									GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(red, green, blue, ambientIntensity, diffuseIntensity)
{
	direction = glm::vec3(xDir, yDir, zDir);
	lightProj = glm::ortho(-5.0f, 5.0f, -5.f, 5.0f, 0.1f, 20.0f);
}

void DirectionalLight::UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation,
	GLint diffuseIntensityLocation, GLint directionLocation)
{
	glUniform3f(ambientColourLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

glm::mat4 DirectionalLight::CalculateLightTransform()
{
	return lightProj * glm::lookAt(-direction, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

DirectionalLight::~DirectionalLight()
{
}
