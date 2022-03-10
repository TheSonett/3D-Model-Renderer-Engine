#version 330 core

layout( location = 0 ) out vec4 color;

in vec2 f_TexCoord;
in vec3 f_Normal;
in vec3 fragPos;
in vec4 DirectionalLightSpacePosition;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light {
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight {
	Light base;
	vec3 direction;
};

struct Material {
	float specularIntensity;
	float shininess;
};

struct PointLight {
	Light base;
	vec3 position;
	float constant;
	float linear;
	float exponent;
};

struct SpotLight {
	PointLight base;
	vec3 direction;
	float edge;
};

uniform sampler2D texture1;
uniform vec3 eyePosition;

uniform DirectionalLight directionalLight;
uniform Material material;

uniform int pointLightCount;
uniform PointLight pointLight[MAX_POINT_LIGHTS];

uniform int spotLightCount;
uniform SpotLight spotLight[MAX_SPOT_LIGHTS];


uniform sampler2D directionalShadowMap;


float CalcDirectionalShadowFactor(DirectionalLight light) 
{
	vec3 projCoords = DirectionalLightSpacePosition.xyz / DirectionalLightSpacePosition.w;
	projCoords = (projCoords * 0.5) + 0.5;

	float closest = texture(directionalShadowMap, projCoords.xy).r;
	float current = projCoords.z;

	float shadow = current > closest ? 1.0 : 0.0;

	return shadow;
}

vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor) {
	vec4 ambientColor = vec4(light.color, 1.0) * light.ambientIntensity;
	float diffuseFactor = max(dot(normalize(f_Normal), normalize(direction)), 0.0); // angle
	vec4 diffuseColor = vec4(light.color, 1.0) * light.diffuseIntensity * diffuseFactor;

	vec4 specularColor = vec4(0, 0, 0, 0);


	if(diffuseFactor > 0.0) {
		vec3 fragToEye = normalize(eyePosition - fragPos);
		vec3 reflectedVertex = normalize(reflect(direction, normalize(f_Normal)));

		float specularFractor = dot(fragToEye, reflectedVertex);
		if(specularFractor > 0.0)
		{
			specularFractor = pow(specularFractor, material.shininess);
			specularColor = vec4(light.color * material.specularIntensity * specularFractor, 1.0);
		}
	}
	return (ambientColor + (1.0 - shadowFactor) * (diffuseColor + specularColor));
}

vec4 CalcDirectionalLight() {
	float shadowFactor = CalcDirectionalShadowFactor(directionalLight);
	return CalcLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}

vec4 CalcPointLight(PointLight pLight)
{
	vec3 direction = fragPos - pLight.position;
	float distance = length(direction);
	direction = normalize(direction);
	
	vec4 colour = CalcLightByDirection(pLight.base, direction, 0.0f);
	float attenuation = pLight.exponent * distance * distance +
						pLight.linear * distance +
						pLight.constant;
	
	return (colour / attenuation);
}

vec4 CalcSpotLight(SpotLight sLight)
{
	vec3 rayDirection = normalize(fragPos - sLight.base.position);
	float slFactor = dot(rayDirection, sLight.direction);
	
	if(slFactor > sLight.edge)
	{
		vec4 colour = CalcPointLight(sLight.base);
		
		return colour;// * (1.0f - (1.0f - slFactor)*(1.0f/(1.0f - sLight.edge)));
		
	} else {
		return vec4(0, 0, 0, 0);
	}
}

vec4 CalcPointLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);
	for(int i = 0; i < pointLightCount; i++)
	{		
		totalColour += CalcPointLight(pointLight[i]);
	}
	
	return totalColour;
}

vec4 CalcSpotLights()
{
	vec4 totalColour = vec4(0, 0, 0, 0);
	for(int i = 0; i < spotLightCount; i++)
	{		
		totalColour += CalcSpotLight(spotLight[i]);
	}
	
	return totalColour;
}

void main()
{
	vec4 finalColor = CalcDirectionalLight();
	finalColor += CalcPointLights();
	finalColor += CalcSpotLights();

	color = texture(texture1, f_TexCoord) * finalColor;
}