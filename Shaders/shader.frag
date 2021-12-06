#version 330


out vec4 fragCol; 

in vec4 posCol; 
in vec2 uvCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 DirectionalLightSpacePos;

uniform sampler2D textureIn;
uniform sampler2D directionalShadowMap;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

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

struct SpotLight{
	PointLight base;
	vec3 direction;
	float edge;
};

struct OmniShadowMap{
	samplerCube shadowMap;
	float farPlane;
};

struct Material{
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];
uniform Material material;

uniform vec3 eyePosition;

float CalcDirectionalShadowFactor(DirectionalLight directionalLight){
	vec3 projCoords = DirectionalLightSpacePos.xyz / DirectionalLightSpacePos.w;
	projCoords = (projCoords * 0.5) + 0.5;
	float current = projCoords.z;
	
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(directionalLight.direction);
	
	float bias = max(0.5 * (1 - dot(normal, lightDir)), 0.005f);
	
	float shadow = 0.0f;
	
	vec2 texelSize = 1.0f/textureSize(directionalShadowMap, 0);
	
	for(int x = -1; x <= 1; ++x){
		for(int y = -1; y <= 1; ++y){
			float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += current - bias> pcfDepth ? 1.0f : 0.0f;;
		}
	}
	shadow /= 9;
	if(projCoords.z > 1){
		shadow = 0.0f;
	}
	return shadow;
}

float CalcOmniShadowFactor(PointLight pLight, int shadowIndex){
	vec3 fragToLight = FragPos - pLight.position;
	float closest = texture(omniShadowMaps[shadowIndex].shadowMap, fragToLight.xyz).r;
}

vec4 CalcLightByDirection(Light light, vec3 direction, float shadowMultiplier){
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
	
	return (ambientColor + (1.0 - shadowMultiplier) * (diffuseColor + specularColor));
}

vec4 CalcDirectionalLight(){
	float directionalShadow = CalcDirectionalShadowFactor(directionalLight);
	return CalcLightByDirection(directionalLight.base, directionalLight.direction, directionalShadow);
}

vec4 CalcPointLight(PointLight pLight){
		vec3 direction = pLight.position - FragPos;
		float distance = length(direction);
		direction = normalize(direction);
		
		vec4 color = CalcLightByDirection(pLight.base, direction, 0.0f);
		float attenuation = pLight.exponent * distance * distance + pLight.linear * distance + pLight.constant;
		return color / attenuation;
}

vec4 CalcSpotLight(SpotLight sLight){
	vec3 rayDirection = normalize(FragPos - sLight.base.position);
	float spotLightFactor = dot(rayDirection, sLight.direction);
	
	vec4 color = vec4(0, 0, 0, 0);
	if(spotLightFactor > sLight.edge){
		color = CalcPointLight(sLight.base);
	}
	return color * (1.0f - (1.0f - spotLightFactor) * (1.0f/(1.0f-sLight.edge)));
}

vec4 CalcSpotLights(){
	vec4 totalColor = vec4(0, 0, 0, 0);
	for(int i = 0; i < spotLightCount; i++){
		totalColor += CalcSpotLight(spotLights[i]);
	}
	return totalColor;
}

vec4 CalcPointLights(){
	vec4 totalColor = vec4(0, 0, 0, 0);
	for(int i = 0; i < pointLightCount; i++){
		totalColor += CalcPointLight(pointLights[i]);
	}
	return totalColor;
}

void main() { 
	vec4 finalColor = CalcDirectionalLight() + CalcPointLights() + CalcSpotLights();
	fragCol = texture(textureIn, uvCoord) * finalColor;
}