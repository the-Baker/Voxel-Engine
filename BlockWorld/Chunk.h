#pragma once
#include "Block.h"
#include <cstdint>
#include <vector>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "shader.h"
#include "GameState.h"
#include "Utility.h"

#define CHUNK_SIZE 8

#define WORLD_WIDTH  16
#define WORLD_HEIGHT 16
#define WORLD_DEPTH  16
#define WORLD_NCHUNKS (WORLD_WIDTH * WORLD_HEIGHT * WORLD_DEPTH)



struct Chunk
{
	std::vector<Block> blocks;
	std::vector<glm::ivec3> blockPositions;
	glm::ivec2 position;
};


void placeBlock(BlockID id, glm::ivec3 pos, Chunk *chunk)
{
	if (!(std::find(chunk->blockPositions.begin(), chunk->blockPositions.end(), pos) != chunk->blockPositions.end()))
	{
		chunk->blocks.push_back(Block{ (uint8_t)id });
		chunk->blockPositions.push_back(glm::ivec3(pos));

		debugState.nBlocks++;
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

void initChunk(glm::ivec2 position, GameState *state)
{
	Chunk chunk = {};
	chunk.position = position;
	
	if (!(std::find(state->activeChunkPositions.begin(), state->activeChunkPositions.end(), position) != state->activeChunkPositions.end()))
	{
		state->chunks.insert(std::make_pair(TwoToOneD(position, CHUNK_SIZE), chunk));
		state->activeChunkPositions.push_back(position);
		debugState.nChunks++;
	}
	//std::cout << "CHUNK INITED AT POSITION: " << chunk.position.x << " " << chunk->position.y << " " << chunk->position.z << std::endl;
}