#pragma once

#include <map>

#include "Utility.h"

#include "Model.h"


#define BLOCK_HALF_SIZE 0.5f
#define BLOCK_SIZE 1.0f

#define	ATLASWIDTH 16
#define ATLASHEIGHT 16

#define ATLASTILEWIDTH (1.0f / (float)ATLASWIDTH)
#define ATLASTILEHEIGHT (1.0f / (float)ATLASHEIGHT)


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

enum BlockID
{
	Air = 0,
	Grass = 1,
	Dirt = 2,
	Stone = 3,
	Bedrock = 4,

	NUM_BLOCK_TYPES
};

enum BlockSide
{
	Top,
	Bottom,
	Front,
	Back,
	Left,
	Right
};

struct BlockData
{
	TexturedModel model;
};

struct BlockDatabase
{
	BlockData *blocks;
};

struct Block
{
	uint8_t id;
};


void loadBlockData(BlockDatabase *database, unsigned int shaderID)
{
	database->blocks = (BlockData*)calloc(NUM_BLOCK_TYPES, sizeof(BlockData));
	//TexturedModel tModel;
	for (int i = 0; i < NUM_BLOCK_TYPES; i++)
	{
		bindAttribute(shaderID, "inPos", 0);
		bindAttribute(shaderID, "inTexCoords", 1);
	}
}


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

glm::vec2 findUV(BlockID id, BlockSide side)
{
	glm::vec2 result;
	switch (id)
	{
		case Grass:
		{
			if (side == Top)
			{
				result.x = 0.0f / ATLASWIDTH;
				result.y = 1.0f;
				return result;
			}
			else if (side == Front || Back || Left || Right)
			{
				result.x = 3.0f / ATLASWIDTH;
				result.y = 1.0f;
				return result;
			}
			else if (side == Bottom)
			{
				result.x = 2.0f / ATLASWIDTH;
				result.y = 1.0f;
				return result;
			}
		}
		case Dirt:
		{
			result.x = 2.0f / ATLASWIDTH;
			result.y = 1.0f;
		}return result;
		case Stone:
		{
			result.x = 1.0f / ATLASWIDTH;
			result.y = 1.0f;
		}return result;
		case Bedrock:
		{
			result.x = 1.0f / ATLASWIDTH;
			result.y = 1.0f / ATLASHEIGHT;
		}return result;
		default:
		{
			return { 0.0f, 0.0f };
		}
	}
}

void addUVs(BlockID id, BlockSide side, std::vector<float> *uvs)
{
	glm::vec2 topLeft = findUV(id, side);
	glm::vec2 topRight = { topLeft.x + ATLASTILEWIDTH, topLeft.y };
	glm::vec2 bottomLeft = { topLeft.x, topLeft.y + ATLASTILEHEIGHT};
	glm::vec2 bottomRight = { topLeft.x + ATLASTILEWIDTH, topLeft.y + ATLASTILEHEIGHT };

	switch (side)
	{
		case Top:
		{
			uvs->push_back(bottomLeft.x);
			uvs->push_back(bottomLeft.y);

			uvs->push_back(bottomRight.x);
			uvs->push_back(bottomRight.y);

			uvs->push_back(topRight.x);
			uvs->push_back(topRight.y);

			uvs->push_back(bottomLeft.x);
			uvs->push_back(bottomLeft.y);

			uvs->push_back(topRight.x);
			uvs->push_back(topRight.y);

			uvs->push_back(topLeft.x);
			uvs->push_back(topLeft.y);
		}return;

		case Bottom:
		{
			uvs->push_back(bottomRight.x);
			uvs->push_back(bottomRight.y);

			uvs->push_back(bottomLeft.x);
			uvs->push_back(bottomLeft.y);

			uvs->push_back(topLeft.x);
			uvs->push_back(topLeft.y);

			uvs->push_back(bottomRight.x);
			uvs->push_back(bottomRight.y);

			uvs->push_back(topLeft.x);
			uvs->push_back(topLeft.y);

			uvs->push_back(topRight.x);
			uvs->push_back(topRight.y);
		}return;

		case Left:
		{
			uvs->push_back(bottomRight.x);
			uvs->push_back(bottomRight.y);

			uvs->push_back(topRight.x);
			uvs->push_back(topRight.y);

			uvs->push_back(topLeft.x);
			uvs->push_back(topLeft.y);

			uvs->push_back(bottomRight.x);
			uvs->push_back(bottomRight.y);

			uvs->push_back(topLeft.x);
			uvs->push_back(topLeft.y);

			uvs->push_back(bottomLeft.x);
			uvs->push_back(bottomLeft.y);
		}return;

		case Right:
		{
			uvs->push_back(bottomLeft.x);
			uvs->push_back(bottomLeft.y);

			uvs->push_back(bottomRight.x);
			uvs->push_back(bottomRight.y);

			uvs->push_back(topRight.x);
			uvs->push_back(topRight.y);

			uvs->push_back(bottomLeft.x);
			uvs->push_back(bottomLeft.y);

			uvs->push_back(topRight.x);
			uvs->push_back(topRight.y);

			uvs->push_back(topLeft.x);
			uvs->push_back(topLeft.y);
		}return;

		case Front:
		{
			uvs->push_back(bottomLeft.x);
			uvs->push_back(bottomLeft.y);

			uvs->push_back(bottomRight.x);
			uvs->push_back(bottomRight.y);

			uvs->push_back(topRight.x);
			uvs->push_back(topRight.y);

			uvs->push_back(bottomLeft.x);
			uvs->push_back(bottomLeft.y);

			uvs->push_back(topRight.x);
			uvs->push_back(topRight.y);

			uvs->push_back(topLeft.x);
			uvs->push_back(topLeft.y);
		}return;

		case Back:
		{
			uvs->push_back(bottomLeft.x);
			uvs->push_back(bottomLeft.y);

			uvs->push_back(bottomRight.x);
			uvs->push_back(bottomRight.y);

			uvs->push_back(topRight.x);
			uvs->push_back(topRight.y);

			uvs->push_back(bottomLeft.x);
			uvs->push_back(bottomLeft.y);

			uvs->push_back(topRight.x);
			uvs->push_back(topRight.y);

			uvs->push_back(topLeft.x);
			uvs->push_back(topLeft.y);
		}return;

		

	}

}

