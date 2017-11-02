#pragma once
#include "Block.h"
#include <cstdint>
#include <vector>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "shader.h"
#include "GameState.h"
#include "Utility.h"

#define GENERATE_HEIGHT 64

#define CHUNK_SIZE 32

#define WORLD_SIZE  8

struct ChunkMesh
{
	std::vector<float> vertices;
	std::vector<float> uvs;
	RawModel model;
};

struct Chunk
{
	std::unordered_map<uint32_t, Block> blocks;
	glm::ivec2 position;
	ChunkMesh mesh;
};

//Position within chunk (0 to CHUNK_SIZE)
void placeBlock(BlockID id, glm::ivec3 pos, Chunk *chunk)
{
	int hashKey = vec3ToInt(pos);
	if (chunk->blocks.count(hashKey) == 0)
	{
		chunk->blocks.insert(std::make_pair(hashKey, Block{ (uint8_t)id }));
		debugState.nBlocks++;
	}
}



void drawChunk(Chunk * chunk, GameState *state, unsigned int shader)
{
	glm::vec3 chunkToWorld = glm::vec3(chunk->position.x * CHUNK_SIZE, 0.0f, chunk->position.y * CHUNK_SIZE);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, chunkToWorld);
	setUniformMat4("model", model, shader);
	drawModel(chunk->mesh.model, state->atlas);
}

void initChunk(glm::ivec2 position, GameState *state)
{
	Chunk chunk = {};
	chunk.position = position;
	long long hashKey = vec2ToInt64(position);
	if (state->chunks.count(hashKey) == 0)
	{
		state->chunks.insert(std::make_pair(vec2ToInt64(position), chunk));
		debugState.nChunks++;
	}
}


glm::ivec2 posOfChunkAtWorldPos(glm::ivec3 playerBlockPos)
{
	return glm::ivec2(playerBlockPos.x / CHUNK_SIZE, playerBlockPos.z / CHUNK_SIZE);
}

void generateChunkMesh(Chunk* chunk)
{
	chunk->mesh.vertices.clear();
	chunk->mesh.uvs.clear();

	for (std::unordered_map<uint32_t, Block>::iterator iter = chunk->blocks.begin(); iter != chunk->blocks.end(); ++iter)
	{

		glm::ivec3 position = intToVec3(iter->first);
		
		auto blockIter = chunk->blocks.find(vec3ToInt(position));
		if (blockIter != chunk->blocks.end())
		{
			Block block = blockIter->second;

			bool leftEmpty =  (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x - 1, position.y, position.z))) == 0);
			bool rightEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x + 1, position.y, position.z))) == 0);
			bool upEmpty =    (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x, position.y + 1, position.z))) == 0);
			bool downEmpty =  (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x, position.y - 1, position.z))) == 0);
			bool frontEmpty = (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x, position.y, position.z + 1))) == 0);
			bool backEmpty =  (chunk->blocks.count(vec3ToInt(glm::ivec3(position.x, position.y, position.z - 1))) == 0);


			if (upEmpty)
			{
				chunk->mesh.vertices.push_back((float)position.x);
				chunk->mesh.vertices.push_back((float)position.y + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.z);

				chunk->mesh.vertices.push_back((float)position.x);
				chunk->mesh.vertices.push_back((float)position.y + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.z + BLOCK_SIZE);

				chunk->mesh.vertices.push_back((float)position.x + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.y + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.z + BLOCK_SIZE);


				chunk->mesh.vertices.push_back((float)position.x);
				chunk->mesh.vertices.push_back((float)position.y + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.z);

				chunk->mesh.vertices.push_back((float)position.x + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.y + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.z + BLOCK_SIZE);

				chunk->mesh.vertices.push_back((float)position.x + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.y + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.z);
				addUVs((BlockID)block.id, Top, &chunk->mesh.uvs);
			}

			if (downEmpty)
			{
				chunk->mesh.vertices.push_back((float)position.x);
				chunk->mesh.vertices.push_back((float)position.y);
				chunk->mesh.vertices.push_back((float)position.z);

				chunk->mesh.vertices.push_back((float)position.x + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.y);
				chunk->mesh.vertices.push_back((float)position.z);

				chunk->mesh.vertices.push_back((float)position.x + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.y);
				chunk->mesh.vertices.push_back((float)position.z + BLOCK_SIZE);


				chunk->mesh.vertices.push_back((float)position.x);
				chunk->mesh.vertices.push_back((float)position.y);
				chunk->mesh.vertices.push_back((float)position.z);

				chunk->mesh.vertices.push_back((float)position.x + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.y);
				chunk->mesh.vertices.push_back((float)position.z +  BLOCK_SIZE);

				chunk->mesh.vertices.push_back((float)position.x);
				chunk->mesh.vertices.push_back((float)position.y);
				chunk->mesh.vertices.push_back((float)position.z + BLOCK_SIZE);
				addUVs((BlockID)block.id, Bottom, &chunk->mesh.uvs);
			}

			if (leftEmpty)
			{
				chunk->mesh.vertices.push_back((float)position.x);
				chunk->mesh.vertices.push_back((float)position.y);
				chunk->mesh.vertices.push_back((float)position.z);

				chunk->mesh.vertices.push_back((float)position.x);
				chunk->mesh.vertices.push_back((float)position.y);
				chunk->mesh.vertices.push_back((float)position.z + BLOCK_SIZE);

				chunk->mesh.vertices.push_back((float)position.x);
				chunk->mesh.vertices.push_back((float)position.y + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.z + BLOCK_SIZE);


				chunk->mesh.vertices.push_back((float)position.x);
				chunk->mesh.vertices.push_back((float)position.y);
				chunk->mesh.vertices.push_back((float)position.z);

				chunk->mesh.vertices.push_back((float)position.x);
				chunk->mesh.vertices.push_back((float)position.y + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.z + BLOCK_SIZE);

				chunk->mesh.vertices.push_back((float)position.x);
				chunk->mesh.vertices.push_back((float)position.y + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.z);
				addUVs((BlockID)block.id, Left, &chunk->mesh.uvs);
			}

			if (rightEmpty)
			{
				chunk->mesh.vertices.push_back((float)position.x + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.y);
				chunk->mesh.vertices.push_back((float)position.z + BLOCK_SIZE);

				chunk->mesh.vertices.push_back((float)position.x + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.y);
				chunk->mesh.vertices.push_back((float)position.z);

				chunk->mesh.vertices.push_back((float)position.x + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.y + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.z);


				chunk->mesh.vertices.push_back((float)position.x + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.y);
				chunk->mesh.vertices.push_back((float)position.z + BLOCK_SIZE);

				chunk->mesh.vertices.push_back((float)position.x + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.y + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.z);

				chunk->mesh.vertices.push_back((float)position.x + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.y + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.z + BLOCK_SIZE);
				addUVs((BlockID)block.id, Right, &chunk->mesh.uvs);
			}

			if (frontEmpty)
			{
				chunk->mesh.vertices.push_back((float)position.x);
				chunk->mesh.vertices.push_back((float)position.y);
				chunk->mesh.vertices.push_back((float)position.z + BLOCK_SIZE);

				chunk->mesh.vertices.push_back((float)position.x + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.y);
				chunk->mesh.vertices.push_back((float)position.z + BLOCK_SIZE);

				chunk->mesh.vertices.push_back((float)position.x + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.y + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.z + BLOCK_SIZE);


				chunk->mesh.vertices.push_back((float)position.x);
				chunk->mesh.vertices.push_back((float)position.y);
				chunk->mesh.vertices.push_back((float)position.z + BLOCK_SIZE);

				chunk->mesh.vertices.push_back((float)position.x + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.y + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.z + BLOCK_SIZE);

				chunk->mesh.vertices.push_back((float)position.x);
				chunk->mesh.vertices.push_back((float)position.y + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.z + BLOCK_SIZE);
				addUVs((BlockID)block.id, Front, &chunk->mesh.uvs);
			}

			if (backEmpty)
			{
				chunk->mesh.vertices.push_back((float)position.x + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.y);
				chunk->mesh.vertices.push_back((float)position.z);

				chunk->mesh.vertices.push_back((float)position.x);
				chunk->mesh.vertices.push_back((float)position.y);
				chunk->mesh.vertices.push_back((float)position.z);

				chunk->mesh.vertices.push_back((float)position.x);
				chunk->mesh.vertices.push_back((float)position.y + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.z);


				chunk->mesh.vertices.push_back((float)position.x + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.y);
				chunk->mesh.vertices.push_back((float)position.z);

				chunk->mesh.vertices.push_back((float)position.x);
				chunk->mesh.vertices.push_back((float)position.y + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.z);

				chunk->mesh.vertices.push_back((float)position.x + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.y + BLOCK_SIZE);
				chunk->mesh.vertices.push_back((float)position.z);
				addUVs((BlockID)block.id, Back, &chunk->mesh.uvs);
			}
		
		}
	}
	chunk->mesh.model = loadToVAO(&chunk->mesh.vertices[0], (unsigned int)chunk->mesh.vertices.size(), &chunk->mesh.uvs[0], (unsigned int)chunk->mesh.uvs.size());
}

glm::ivec3 worldToChunkPosition(glm::ivec3 worldPos, glm::ivec2 chunkPos)
{
	glm::ivec3 result;
	result.x = worldPos.x - (chunkPos.x * CHUNK_SIZE);
	result.y = worldPos.y;
	result.z = worldPos.z - (chunkPos.y * CHUNK_SIZE);
	return result;
}

void playerPlaceBlock(BlockID id, glm::ivec3 pos, Chunk *chunk)
{
	glm::ivec3 positionToPlace = worldToChunkPosition(pos, chunk->position);
	int hashKey = vec3ToInt(positionToPlace);
	if (chunk->blocks.count(hashKey) == 0)
	{
		chunk->blocks[hashKey] = Block{ (uint8_t)id };
		debugState.nBlocks++;
		generateChunkMesh(chunk);
		std::cout << "Block Placed At: " << pos.x << " " << pos.y << " " << pos.z << std::endl; 
	}
}

void removeBlock(glm::ivec3 pos, Chunk *chunk)
{
	glm::ivec3 blockPos = worldToChunkPosition(pos, chunk->position);
	int hashKey = vec3ToInt(blockPos);
	if (chunk->blocks.count(hashKey) == 1)
	{
		chunk->blocks.erase(hashKey);
		debugState.nChunks--;
		generateChunkMesh(chunk);
		std::cout << "removed block at: " << pos.x << " " << pos.y << " " << pos.z << std::endl;
	}
}

Chunk* findChunkAtWorldPos(GameState *gamestate, glm::ivec3 position)
{
	glm::ivec2 chunkPos;
	chunkPos.x = (position.x / CHUNK_SIZE);
	chunkPos.y = (position.z / CHUNK_SIZE);

	long long int hashKey = vec2ToInt64(chunkPos);
	if (gamestate->chunks.count(hashKey) == 1)
	{
		return &gamestate->chunks.at(hashKey);
	}
	else
	{
		return nullptr;
	}
}


bool checkChunkExists(glm::ivec2 position, GameState *state)
{
	int count = (int)state->chunks.count(vec2ToInt64(position));
	return (count > 0);
}