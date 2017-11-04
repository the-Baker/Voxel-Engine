#version 330 core

out vec4 fragColor;

in vec2 texCoords;
in float lightValue;

uniform sampler2D texture1;

void main()
{
	fragColor = vec4(texture(texture1, texCoords) * lightValue);
}