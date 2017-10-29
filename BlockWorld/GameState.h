#pragma once
#include <unordered_map>
#include "Player.h"
#include "Block.h"

struct Chunk;
struct Ray
{
	glm::vec3 origin;
	glm::vec3 end;
	glm::vec3 direction;
};

enum GAME_MODE
{
	MENU,
	WORLD
};

struct GameState
{
	GAME_MODE mode;
	Player player;
	bool gameShouldRun = true;
	bool shouldGenerate = true;
	bool goFast = true;
	bool firstRun = true;
	float deltaTime = 0.0f;
	float lastTime = 0.0f;
	float currentTime = 0.0f;
	std::unordered_map<long long int, Chunk> chunks;
	BlockDatabase bDatabase;
	unsigned int atlas = loadTexture("MinecraftTextureAtlas.png");
	Ray playerRay;
	glm::vec3 focusedBlockPos;
	BlockID blockTypeToPlace = Grass;
};


struct DebugState
{
	bool isWireFrame = false;
	int nBlocks = 0;
	int nChunks = 0;
	int nVertices = 0;
};