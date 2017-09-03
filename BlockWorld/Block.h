#pragma once

#include "Utility.h"

#include "Model.h"

float vertices[] =
{
	-0.5f, 0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,
	0.5f,-0.5f,-0.5f,
	0.5f, 0.5f,-0.5f,

	-0.5f, 0.5f, 0.5f,
	-0.5f,-0.5f, 0.5f,
	0.5f,-0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,

	0.5f, 0.5f,-0.5f,
	0.5f,-0.5f,-0.5f,
	0.5f,-0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,

	-0.5f, 0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,
	-0.5f,-0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,

	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f,-0.5f,
	0.5f, 0.5f,-0.5f,
	0.5f, 0.5f, 0.5f,

	-0.5f,-0.5f, 0.5f,
	-0.5f,-0.5f,-0.5f,
	0.5f,-0.5f,-0.5f,
	0.5f,-0.5f, 0.5f
};

float uvs[] =
{
	0,0,
	0,1,
	1,1,
	1,0,
	0,0,
	0,1,
	1,1,
	1,0,
	0,0,
	0,1,
	1,1,
	1,0,
	0,0,
	0,1,
	1,1,
	1,0,
	0,0,
	0,1,
	1,1,
	1,0,
	0,0,
	0,1,
	1,1,
	1,0
};

unsigned int indices[] =
{
	0,1,3,
	3,1,2,
	4,5,7,
	7,5,6,
	8,9,11,
	11,9,10,
	12,13,15,
	15,13,14,
	16,17,19,
	19,17,18,
	20,21,23,
	23,21,22
};


enum ToolRequired
{
	WOOD,
	STONE,
	IRON,
	GOLD,
	DIAMOND
};

enum BlockID
{
	Air = 0,
	Grass = 1,
	Dirt = 2,
	StoneBrick = 3,
	Bedrock = 4,

	NUM_BLOCK_TYPES
};

struct BlockData
{
	TexturedModel model;
};

struct BlockDatabase
{
	BlockData *blocks;
};

void loadBlockData(BlockDatabase *database, unsigned int shaderID)
{
	database->blocks = (BlockData*)calloc(NUM_BLOCK_TYPES, sizeof(BlockData));
	TexturedModel tModel;
	for (int i = 0; i < NUM_BLOCK_TYPES; i++)
	{
		std::string fileName = std::to_string(i) + ".png";
		tModel.model = loadToVAO(vertices, ARRAY_COUNT(vertices), indices, ARRAY_COUNT(indices), uvs, ARRAY_COUNT(uvs));
		bindAttribute(shaderID, "inPos", 0);
		bindAttribute(shaderID, "inTexCoords", 1);
		tModel.texture.id = loadTexture(fileName.c_str());
		database->blocks[i].model = tModel;
	}
}

struct Block
{
	BlockID id;
};

BlockData getBlockData(Block block, BlockDatabase *database)
{
	return database->blocks[block.id];
}

TexturedModel getBlockModel(Block block, BlockDatabase *database)
{
	return getBlockData(block, database).model;
}

void drawBlock(Block block, BlockDatabase *database)
{
	TexturedModel model = getBlockModel(block, database);
	drawTexturedModel(model);
}

void placeBlock(BlockID id, glm::ivec3 pos, std::vector<Block> *blocks, std::vector<glm::ivec3> *usedPositions)
{
	if (!(std::find(usedPositions->begin(), usedPositions->end(), pos) != usedPositions->end()))
	{
		blocks->push_back(Block{ id });
		usedPositions->push_back(glm::ivec3(pos));
	}
}
