#include "SpotLight.h"

SpotLight::SpotLight() 
	: PointLight() {
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	edge = 0.0f;
	cutOff = glm::cos(glm::radians(edge));
}

SpotLight::SpotLight(GLfloat red, GLfloat green, GLfloat blue,
	GLfloat ambientIntensity, GLfloat diffuseIntensity, 
	GLfloat xPos, GLfloat yPos, GLfloat zPos, 
	GLfloat xDir, GLfloat yDir, GLfloat zDir, 
	GLfloat con, GLfloat lin, GLfloat exp, GLfloat edge) : PointLight(red, green, blue,
																	  ambientIntensity, diffuseIntensity, 
																	  xPos, yPos, zPos, 
																	  con,lin, exp)
{
	direction = glm::vec3(xDir, yDir, zDir);
	this->edge = edge;
	cutOff = glm::cos(glm::radians(edge));
}


void SpotLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, 
	GLuint diffuseIntensityLocation, GLuint positionLocation, 
	GLuint directionLocation, GLuint edgeLocation, 
	GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation)
{

	glUniform3f(ambientColourLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);

	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation, linear);
	glUniform1f(exponentLocation, exponent);


	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(edgeLocation, cutOff);
}

SpotLight::~SpotLight() {}