#pragma once

#include <SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

char* file_read(const char* filename);


GLuint createShader(const char* vertexPath, const char* fragmentPath)
{
	char* vShaderData = file_read(vertexPath);
	char* fShaderData = file_read(fragmentPath);
	char infoLog[512];
	int success;
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderData, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderData, NULL);
	glCompileShader(fragmentShader);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return shaderProgram;
}


//@Speed Uniform Locations can be optained once instead of every frame
void setUniformInt(char* uniformName, int value, GLuint program)
{
	int location = glGetUniformLocation(program, uniformName);
	glUniform1i(location, value);
}

void setUniformMat4(char* uniformName, glm::mat4 matrix, GLuint program)
{
	int location = glGetUniformLocation(program, uniformName);
	glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}
void setUniformVec3(char* uniformName, glm::vec3 vector, GLuint program)
{
	int location = glGetUniformLocation(program, uniformName);
	glUniform3f(location, vector.x, vector.y, vector.z);
}
void setUniformFloat(char* uniformName, float f, GLuint program)
{
	int location = glGetUniformLocation(program, uniformName);
	glUniform1f(location, f);
}
void setUniformUInt(char* uniformName, unsigned int i, GLuint program)
{
	int location = glGetUniformLocation(program, uniformName);
	glUniform1f(location, i);
}

char* file_read(const char* filename) {
	SDL_RWops *rw = SDL_RWFromFile(filename, "rb");
	if (rw == NULL) return NULL;

	Sint64 res_size = SDL_RWsize(rw);
	char* res = (char*)malloc(res_size + 1);

	Sint64 nb_read_total = 0, nb_read = 1;
	char* buf = res;
	while (nb_read_total < res_size && nb_read != 0) {
		nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
		nb_read_total += nb_read;
		buf += nb_read;
	}
	SDL_RWclose(rw);
	if (nb_read_total != res_size) {
		free(res);
		return NULL;
	}

	res[nb_read_total] = '\0';
	return res;
}

