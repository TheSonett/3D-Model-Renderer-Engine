#pragma once

#include <GL/glew.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Light {
public:
	Light();
	Light(float red, float green, float blue, float ambientIntensity, float diffuseIntensity);
	~Light();
protected:
	glm::vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
	glm::mat4 lightProj;
};