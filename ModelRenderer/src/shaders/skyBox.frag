#version 330

layout (location = 0) out vec4 color;

in vec3 texCoords;

uniform samplerCube skybox;

void main() 
{
	color = texture(skybox, texCoords);
}