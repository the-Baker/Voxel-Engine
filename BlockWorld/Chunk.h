#pragma once
#include "Block.h"
#include <cstdint>
#include <vector>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "shader.h"
#include "GameState.h"
#include "Utility.h"

#define CHUNK_SIZE 2

#define WORLD_SIZE  2



#define X_BIT_OFFSET 24
#define Y_BIT_OFFSET 16
#define Z_BIT_OFFSET 8
#define E_BIT_OFFSET 0


struct ChunkMesh
{
	std::vector<float> vertices;
	RawModel model;
};

struct Chunk
{
	std::unordered_map<uint32_t, Block> blocks;
//	std::vector<Block> blocks;
	std::vector<glm::ivec3> blockPositions;
	glm::ivec2 position;
	ChunkMesh mesh;
};


void placeBlock(BlockID id, glm::ivec3 pos, Chunk *chunk)
{
	int hashKey = vec3ToInt(pos);
	if (chunk->blocks.count(hashKey) > 0)
	{
		chunk->blocks.insert(std::make_pair(hashKey, Block{ (uint8_t)id }));
		chunk->blockPositions.push_back(glm::ivec3(pos));
		debugState.nBlocks++;
	}
}



void drawChunk(Chunk * chunk, BlockDatabase *bDatabase, unsigned int shader)
{
	for (int i = 0; i < chunk->blockPositions.size(); i++)
	{
		glm::vec3 position = chunk->blockPositions[i];
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		setUniformMat4("model", model, shader);
		drawModel(chunk->mesh.model);
		//drawBlock(chunk->blocks.at(vec3ToInt(position)), bDatabase);
	}
}

void initChunk(glm::ivec2 position, GameState *state)
{
	Chunk chunk = {};
	chunk.position = position;
	
	if (!(std::find(state->activeChunkPositions.begin(), state->activeChunkPositions.end(), roundToMultiple(position, CHUNK_SIZE)) != state->activeChunkPositions.end()))
	{
		state->chunks.insert(std::make_pair(TwoToOneD(roundToMultiple(position, CHUNK_SIZE), CHUNK_SIZE), chunk));
		state->activeChunkPositions.push_back(roundToMultiple(position, CHUNK_SIZE));
		debugState.nChunks++;
		std::cout << "CHUNK INITED AT POSITION: " << chunk.position.x << " " << chunk.position.y << " " << debugState.nChunks << std::endl;
	}
}




void generateChunkMesh(Chunk* chunk)
{
	for (int i = 0; i < chunk->blockPositions.size(); i++)
	{
		glm::ivec3 position = chunk->blockPositions[i];
		if (position.x > 0 && position.x < CHUNK_SIZE && position.y > 0 && position.y < CHUNK_SIZE && position.z > 0 && position.z < CHUNK_SIZE)
		{
			if (!(chunk->blocks.at(vec3ToInt(glm::ivec3(position.x - 1, position.y, position.z))).id > 0))
			{
				chunk->mesh.vertices.push_back(position.x);
				chunk->mesh.vertices.push_back(position.y);
				chunk->mesh.vertices.push_back(position.z);

				chunk->mesh.vertices.push_back(position.x);
				chunk->mesh.vertices.push_back(position.y + 1.0f);
				chunk->mesh.vertices.push_back(position.z);

				chunk->mesh.vertices.push_back(position.x);
				chunk->mesh.vertices.push_back(position.y + 1.0f);
				chunk->mesh.vertices.push_back(position.z - 1.0f);

				chunk->mesh.vertices.push_back(position.x);
				chunk->mesh.vertices.push_back(position.y);
				chunk->mesh.vertices.push_back(position.z);

				chunk->mesh.vertices.push_back(position.x);
				chunk->mesh.vertices.push_back(position.y + 1.0f);
				chunk->mesh.vertices.push_back(position.z - 1.0f);

				chunk->mesh.vertices.push_back(position.x);
				chunk->mesh.vertices.push_back(position.y);
				chunk->mesh.vertices.push_back(position.z - 1.0f);
			}
			if (!(chunk->blocks.at(vec3ToInt(glm::ivec3(position.x, position.y - 1, position.z))).id > 0))
			{
				chunk->mesh.vertices.push_back(position.x);
				chunk->mesh.vertices.push_back(position.y);
				chunk->mesh.vertices.push_back(position.z);

				chunk->mesh.vertices.push_back(position.x);
				chunk->mesh.vertices.push_back(position.y);
				chunk->mesh.vertices.push_back(position.z - 1.0f);

				chunk->mesh.vertices.push_back(position.x + 1.0f);
				chunk->mesh.vertices.push_back(position.y);
				chunk->mesh.vertices.push_back(position.z - 1.0f);

				chunk->mesh.vertices.push_back(position.x);
				chunk->mesh.vertices.push_back(position.y);
				chunk->mesh.vertices.push_back(position.z);

				chunk->mesh.vertices.push_back(position.x + 1.0f);
				chunk->mesh.vertices.push_back(position.y);
				chunk->mesh.vertices.push_back(position.z);

				chunk->mesh.vertices.push_back(position.x + 1.0f);
				chunk->mesh.vertices.push_back(position.y);
				chunk->mesh.vertices.push_back(position.z - 1.0f);
			}
		}
	}

	chunk->mesh.model = loadToVAO(&chunk->mesh.vertices[0], chunk->mesh.vertices.size(), 0, 0);
}
