#version 330

layout(location = 0) in vec3 pos;

uniform mat4 projection;
uniform mat4 view;

out vec3 TexCoord;

void main(){
	TexCoord = pos;
	gl_Position = projection * view * vec4(pos, 1);
}