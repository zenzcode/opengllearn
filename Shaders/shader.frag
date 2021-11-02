#version 330


out vec4 fragCol; 

in vec4 posCol; 
in vec2 uvCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D textureIn;

struct DirectionalLight{
	vec3 color;
	float ambientIntensity; 
	vec3 direction;
	float diffuseIntensity;
};

struct Material{
	float specularIntensity;
	float shininess;
};

uniform DirectionalLight directionalLight;
uniform Material material;

uniform vec3 eyePosition;

void main() { 
	vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;
	float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
	vec4 diffuseColor = vec4(directionalLight.color, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;
	vec4 specularColor = vec4(0, 0, 0, 0);
	if(diffuseFactor > 0.0f){
		vec3 fragToEye = normalize(eyePosition - FragPos);
		vec3 reflectedVertex = normalize(reflect(directionalLight.direction, normalize(Normal)));
		float specularFactor = dot(fragToEye, reflectedVertex);
		if(specularFactor > 0){
			specularFactor = pow(specularFactor, material.shininess);
			specularColor = vec4(directionalLight.color * material.specularIntensity * specularFactor, 1.0f);
		}
	}
	fragCol = texture(textureIn, uvCoord) * (ambientColor + diffuseColor + specularColor);
}