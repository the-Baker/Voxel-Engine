#version 330 core

layout (location = 0)in vec3 inPos;
layout (location = 1)in vec2 inTexCoords;
layout (location = 2)in float inLightValue;

out vec2 texCoords;
out float lightValue;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
	gl_Position = projection * view * model * vec4(inPos, 1.0);
	texCoords = inTexCoords;
	lightValue = inLightValue;
}
