#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;

out vec3 fragPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	normal = mat3(transpose(inverse(model))) * inNormal;
	fragPos = vec3(model * vec4(inPos, 1.0));

	gl_Position = projection * view * model * vec4(inPos, 1.0);
}
