#pragma once


#include <glm/glm.hpp>
#include "PointLight.h"

class SpotLight : public PointLight 
{
public:
	SpotLight();
	SpotLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat ambientIntensity, GLfloat diffuseIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat xDir, GLfloat yDir, GLfloat zDir,
		GLfloat con, GLfloat lin, GLfloat exp, GLfloat edge);
	virtual ~SpotLight();
public:
	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation,
		GLuint directionLocation, GLuint edgeLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

private:
	glm::vec3 direction;
	float edge, cutOff;
};