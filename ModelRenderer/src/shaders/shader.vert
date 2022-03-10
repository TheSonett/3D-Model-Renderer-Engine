#version 330 core

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec2 v_TexCoord;
layout( location = 2 ) in vec3 v_Normal;

out vec2 f_TexCoord;
out vec3 f_Normal;
out vec3 fragPos;
out vec4 DirectionalLightSpacePosition;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 directionalLightTransform;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	DirectionalLightSpacePosition = directionalLightTransform * model * vec4(position, 1.0);

	f_TexCoord = v_TexCoord;

	f_Normal = mat3(transpose(inverse(model))) * v_Normal;
	fragPos = (model * vec4(position, 1.0)).xyz;
}