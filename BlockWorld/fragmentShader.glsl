#version 330 core
out vec4 FragColor;

in vec2 texCoord;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform sampler2D ourTexture;

void main()
{
	FragColor = vec4(objectColor * lightColor,1.0);
}