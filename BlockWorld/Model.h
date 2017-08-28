#pragma once
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include "mesh.h"

std::vector<Texture> textures_loaded;

struct Model
{
	std::vector<Mesh> meshes;
	std::string directory;
};

Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene, Model *model);
std::vector<Texture> LoadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName, Model *model);

void ProcessNode(aiNode *node, const aiScene *scene, Model *model)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		model->meshes.push_back(ProcessMesh(mesh, scene, model));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, model);
	}
}

unsigned int loadTexture(const char* path, const std::string &directory, bool gamma = false)
{
	std::string filename = directory + '\\' + std::string(path);

	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;

	unsigned char * data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "FAILURE: FAILED TO LOAD TEXTURE " << filename << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}
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
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "FAILURE: FAILED TO LOAD TEXTURE" << filename << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}


std::vector<Texture> LoadMaterialTextures(aiMaterial *material, aiTextureType type, std::string typeName, Model *model)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
	{
		aiString str;
		material->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture texture;
			texture.id = loadTexture(str.C_Str(), model->directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}

void LoadModel(std::string path, Model *model)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	if (!scene || scene->mFlags * AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	model->directory = path.substr(0, path.find_last_of('\\'));
	ProcessNode(scene->mRootNode, scene, model);
}

void DrawModel(unsigned int shader, Model *model)
{
	for (int i = 0; i < model->meshes.size(); i++)
	{
		DrawMesh(shader, &model->meshes[i]);
	}
}

Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene, Model *model)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoords = vec;
		}
		else
		{
			vertex.texCoords = glm::vec2(0.0f, 0.0f);
		}
		/*
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.tangent = vector;

		vector.x = mesh->mBitangents[i].x;
		vector.y = mesh->mBitangents[i].y;
		vector.z = mesh->mBitangents[i].z;
		vertex.bitangent = vector; 
		*/

		vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];


	std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", model);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", model);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	

	return Mesh(vertices, indices, textures);
}