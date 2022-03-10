#include "Light.h"

Light::Light() 
{
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
	diffuseIntensity = 0.0f;
}

Light::Light(float red, float green, float blue, float ambientIntensity, float diffuseIntensity)
{
	color = glm::vec3(red, green, blue);
	this->ambientIntensity = ambientIntensity;
	this->diffuseIntensity = diffuseIntensity;
}

Light::~Light() {}