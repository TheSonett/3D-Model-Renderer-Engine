#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight();
	PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat ambientIntensity, GLfloat diffuseIntensity, GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat con, GLfloat lin, GLfloat exp);
	virtual ~PointLight();
public:
	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint diffuseIntensityLocation, GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);
protected:
	glm::vec3 position;
	GLfloat constant, linear, exponent;
};

