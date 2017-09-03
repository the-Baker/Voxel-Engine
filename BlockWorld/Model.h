#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


unsigned int loadTexture(char const *filename)
{

	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;

	unsigned char * data = stbi_load(filename, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "FAILURE: FAILED TO LOAD TEXTURE" << filename << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}

struct Texture2D
{
	unsigned int id;
};

struct RawModel
{
	unsigned int vaoID;
	unsigned int indexCount;
};

struct TexturedModel
{
	RawModel model;
	Texture2D texture;
};

void drawModel(RawModel model)
{
	glBindVertexArray(model.vaoID);
	glEnableVertexAttribArray(0);
	glDrawElements(GL_TRIANGLES, model.indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void drawTexturedModel(TexturedModel model)
{
	glBindVertexArray(model.model.vaoID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model.texture.id);
	glDrawElements(GL_TRIANGLES, model.model.indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}



void storeDataInAttributeList(float *data, unsigned int dataCount, unsigned int attributeNumber, unsigned int dimensions)
{
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * dataCount, data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(attributeNumber);
	glVertexAttribPointer(attributeNumber, dimensions, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void bindAttribute(unsigned int shader, char* variableName, int attributeNumber)
{
	glBindAttribLocation(shader, attributeNumber, variableName);
}

void bindIndexBuffer(unsigned int *indices, unsigned int indexCount)
{
	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, indices, GL_STATIC_DRAW);
}

RawModel loadToVAO(float *vertices, unsigned int vertexCount, unsigned int *indices, unsigned int indexCount, float *uvs, unsigned int uvCount)
{
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	storeDataInAttributeList(vertices, vertexCount, 0, 3);
	storeDataInAttributeList(uvs, uvCount, 1, 2);

	bindIndexBuffer(indices, indexCount);
	glBindVertexArray(0);
	return RawModel({ vao, indexCount });
}

