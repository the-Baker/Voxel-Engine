#pragma once
#include <unordered_map>
#include "Player.h"
#include "Block.h"

struct Chunk;

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
	bool firstRun = true;
	float deltaTime = 0.0f;
	float lastTime = 0.0f;
	float currentTime = 0.0f;
	std::unordered_map<int, Chunk> chunks;
	std::vector<glm::ivec2> activeChunkPositions;
	BlockDatabase bDatabase;
	unsigned int atlas = loadTexture("MinecraftTextureAtlas.png");
};


struct DebugState
{
	bool isWireFrame = false;
	int nBlocks = 0;
	int nChunks = 0;
	int nVertices = 0;
};