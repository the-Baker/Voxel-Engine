#pragma once
#include <SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <string.>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "shader.h"

struct Vertex 
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

struct Texture
{
	GLuint id;
	std::string type;
	aiString path;
};

struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indicies;
	std::vector<Texture> textures;
	GLuint vao;
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indicies = indices;
		this->textures = textures;
		setupMesh();
	}
	GLuint vbo;
	GLuint ebo;
	void setupMesh();
};

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int), &indicies[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::texCoords));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::tangent));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::bitangent));

	glBindVertexArray(0);
}

void DrawMesh(unsigned int shader, Mesh *mesh)
{
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;
	GLuint normalNr = 1;
	GLuint heightNr = 1;
	for (unsigned int i = 0; i < mesh->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::stringstream ss;
		std::string number;
		std::string name = mesh->textures[i].type;
		if (name == "texture_diffuse")
			ss << diffuseNr++;
		else if (name == "texture_specular")
			ss << specularNr++;
		else if (name == "texture_normal")
			ss << normalNr++;
		else if (name == "texture_height")
			ss << heightNr++;


		number == ss.str();

		glUniform1i(glGetUniformLocation(shader, (name + ss.str()).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, mesh->textures[i].id);
	}

	glBindVertexArray(mesh->vao);
	glDrawElements(GL_TRIANGLES, mesh->indicies.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}
