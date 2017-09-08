#pragma once
#include "Block.h"
#include <cstdint>
#include <vector>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "shader.h"

#define CHUNK_SIZE 16

#define WORLD_WIDTH 8
#define WORLD_HEIGHT 8
#define WORLD_DEPTH 8
#define WORLD_NCHUNKS (WORLD_WIDTH * WORLD_HEIGHT * WORLD_DEPTH)



struct Chunk
{
	std::vector<Block> blocks;
	std::vector<glm::ivec3> blockPositions;
	glm::ivec3 position;
};

Chunk* findChunkAtWorldPos(Chunk *chunks, glm::ivec3 worldPosition)
{
	int x = roundFloatToInt((float)worldPosition.x / (float)CHUNK_SIZE);
	int y = roundFloatToInt((float)worldPosition.y / (float)CHUNK_SIZE);
	int z = roundFloatToInt((float)worldPosition.z / (float)CHUNK_SIZE);

	return chunks + (z * WORLD_HEIGHT * WORLD_WIDTH + y * WORLD_WIDTH + x);
}


void placeBlock(BlockID id, glm::ivec3 pos, Chunk *chunk)
{
	if (!(std::find(chunk->blockPositions.begin(), chunk->blockPositions.end(), pos) != chunk->blockPositions.end()))
	{
		std::cout << "PLACE BLOCK AT " << pos.x << " " << pos.y << " " << pos.z << std::endl;
		std::cout << "CHUNK POS " << chunk->position.x << " " << chunk->position.y << " " << chunk->position.z << std::endl;
		chunk->blocks.push_back(Block{ id });
		chunk->blockPositions.push_back(glm::ivec3(pos));
	}
}


void drawChunk(Chunk * chunk, BlockDatabase *bDatabase, unsigned int shader)
{
	for (int i = 0; i < chunk->blocks.size(); i++)
	{
		glm::vec3 position = chunk->blockPositions[i];
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, (glm::vec3)chunk->blockPositions[i]);
		setUniformMat4("model", model, shader);
		drawBlock(chunk->blocks[i], bDatabase);
	}
}