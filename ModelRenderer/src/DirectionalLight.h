#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity, GLfloat diffuseIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir);
	~DirectionalLight();
public:
	void UseLight(GLint ambientIntensityLocation, GLint ambientColourLocation, GLint diffuseIntensityLocation, GLint directionLocation);
	glm::mat4 CalculateLightTransform();
private:
	glm::vec3 direction;
};

