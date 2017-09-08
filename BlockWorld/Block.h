#pragma once

#include "Utility.h"

#include "Model.h"

#define GENERATE_HALF_SIZE 4


float vertices[] =
{
	-.5,-.5,-.5,
	-.5,.5,-.5,
	.5,.5,-.5,
	.5,.5,-.5,
	.5,-.5,-.5,
	-.5,-.5,-.5,

	.5,-.5,-.5,
	.5,.5,-.5,
	.5,.5,.5,
	.5,.5,.5,
	.5,-.5,.5,
	.5,-.5,-.5,

	.5,-.5,.5,
	.5,.5,.5,
	-.5,.5,.5,
	-.5,.5,.5,
	-.5,-.5,.5,
	.5,-.5,.5,

	-.5,-.5,.5,
	-.5,.5,.5,
	-.5,.5,-.5,
	-.5,.5,-.5,
	-.5,-.5,-.5,
	-.5,-.5,.5,

	-.5,.5,-.5,
	-.5,.5,.5,
	.5,.5,.5,
	.5,.5,.5,
	.5,.5,-.5,
	-.5,.5,-.5,

	-.5,-.5,-.5,
	.5,-.5,-.5,
	.5,-.5,.5,
	.5,-.5,.5,
	-.5,-.5,.5,
	-.5,-.5,-.5
};

float uvs[] =
{
	0,1,
	0,0,
	1,0,
	1,0,
	1,1,
	0,1,

	0,1,
	0,0,
	1,0,
	1,0,
	1,1,
	0,1,

	0,1,
	0,0,
	1,0,
	1,0,
	1,1,
	0,1,

	0,1,
	0,0,
	1,0,
	1,0,
	1,1,
	0,1,

	0,1,
	0,0,
	1,0,
	1,0,
	1,1,
	0,1,

	0,1,
	0,0,
	1,0,
	1,0,
	1,1,
	0,1
};

/*
unsigned int indices[] =
{
	// front
	0, 1, 2,
	2, 3, 0,
	// top
	1, 5, 6,
	6, 2, 1,
	// back
	7, 6, 5,
	5, 4, 7,
	// bottom
	4, 0, 3,
	3, 7, 4,
	// left
	4, 5, 1,
	1, 0, 4,
	// right
	3, 2, 6,
	6, 7, 3,
};
*/


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
		tModel.model = loadToVAO(vertices, ARRAY_COUNT(vertices), uvs, ARRAY_COUNT(uvs));
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

