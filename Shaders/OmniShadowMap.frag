#version 330

in vec4 FragPos;

uniform vec3 lightPos;
uniform float farPlane; 

out vec4 color;


void main(){
	float distance = length(FragPos.xyz - lightPos);
	distance /= farPlane;
	gl_FragDepth = distance;
}