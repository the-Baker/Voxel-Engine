#version 330 core

in vec3 inPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	gl_Position = projection * view * model * vec4(inPos, 1.0);
}
