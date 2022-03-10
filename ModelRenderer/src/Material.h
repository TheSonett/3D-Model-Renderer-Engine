#pragma once

#include <GL/glew.h>

class Material {
public:
	Material();
	Material(float specularIntensity, float shininess);
	~Material();
public:
	void UseMaterial(unsigned int specularIntensityLocation, unsigned int shininessLocation);
private:
	float specularIntensity;
	float shininess;
};