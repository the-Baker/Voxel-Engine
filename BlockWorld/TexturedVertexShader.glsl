#version 330 core

in vec3 inPos;
in vec2 inTexCoords;

out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	gl_Position = projection * view * model * vec4(inPos, 1.0);
	texCoords = inTexCoords;
}
