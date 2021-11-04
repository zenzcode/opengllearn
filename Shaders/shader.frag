#version 330


out vec4 fragCol; 

in vec4 posCol; 
in vec2 uvCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D textureIn;

const int MAX_POINT_LIGHTS = 3;

struct Light{
	vec3 color;
	float ambientIntensity; 
	float diffuseIntensity;
};

struct DirectionalLight{
	Light base;
	vec3 direction;
};

struct PointLight{
	Light base;
	vec3 position;
	float constant;
	float exponent;
	float linear;
};

struct Material{
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform Material material;

uniform vec3 eyePosition;

vec4 CalcLightByDirection(Light light, vec3 direction){
	vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;
	float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
	vec4 diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;
	vec4 specularColor = vec4(0, 0, 0, 0);
	if(diffuseFactor > 0.0f){
		vec3 fragToEye = normalize(eyePosition - FragPos);
		
		vec3 reflectedVertex = normalize(reflect(-direction, normalize(Normal)));
		float specularFactor = dot(fragToEye, reflectedVertex);
		if(specularFactor > 0){
			specularFactor = pow(specularFactor, material.shininess);
			specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
		}
	}
	
	return (ambientColor + diffuseColor + specularColor);
}

vec4 CalcDirectionalLight(){
	return CalcLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 CalcPointLight(){
	vec4 totalColor = vec4(0, 0, 0, 0);
	for(int i = 0; i < pointLightCount; i++){
		vec3 direction = pointLights[i].position - FragPos;
		float distance = length(direction);
		direction = normalize(direction);
		
		vec4 color = CalcLightByDirection(pointLights[i].base, direction);
		float attenuation = pointLights[i].exponent * distance * distance + pointLights[i].linear * distance + pointLights[i].constant;
		totalColor += (color / attenuation);
	}
	return totalColor;
}

void main() { 
	vec4 finalColor = CalcDirectionalLight() + CalcPointLight();
	fragCol = texture(textureIn, uvCoord) * finalColor;
}