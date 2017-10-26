#include <SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <unordered_map>
#include <stdio.h>
#include <thread>
#include <string>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Utility.h"
#include "simplexnoise.h"

#include "Player.h"
#include "Model.h"
#include "Block.h"
#include "Chunk.h"
#include "GameState.h"
#include "shader.h"
#include "camera.h"



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

glm::ivec3 worldToChunkPosition(glm::ivec3 worldPos, glm::ivec2 chunkPos)
{
	glm::ivec3 result;
	result.x = worldPos.x - (chunkPos.x * CHUNK_SIZE);
	result.y = worldPos.y;
	result.z = worldPos.z - (chunkPos.y * CHUNK_SIZE);
	return result;
}

bool checkChunkExists(glm::ivec2 position, GameState *state)
{
	int count = state->chunks.count(vec2ToInt64(position));
	return (count > 0);
}


void generateChunk(GameState *state, glm::ivec2 position)
{
	initChunk(position, state);
	glm::ivec3 chunkBottomLeft(position.x, 0, position.y);
	Chunk *chunk = &state->chunks.at(vec2ToInt64(position));
	for (int z = 0; z < CHUNK_SIZE; z++)
	{
		for (int y = 0; y < 1; y++)
		{
			for (int x = 0; x < CHUNK_SIZE; x++)
			{
				int height = scaled_octave_noise_2d(5.0f, 7.0f, 0.0005f, 16.0f, 48.0f, (float)(chunkBottomLeft.x * CHUNK_SIZE + x), (float)(chunkBottomLeft.z * CHUNK_SIZE + z));
				for (int i = 0; i < height; i++)
				{
					glm::ivec3 positionToPlace = glm::ivec3(x, i, z);


					///*
					if (i >= height - 1)
						placeBlock(Grass, positionToPlace, chunk);
					else if (i > height - 6)
						placeBlock(Dirt, positionToPlace, chunk);
					else if (i == 0)
						placeBlock(Bedrock, positionToPlace, chunk);
					else
						placeBlock(Stone, positionToPlace, chunk);

				}
			}
		}
	}
	generateChunkMesh(chunk);
}

void generateWorldAroundPosition(GameState *state, glm::ivec2 pos, int size)
{
	for (int z = 0; z < size; z++)
	{
		for (int x = 0; x < size; x++)
		{
			generateChunk(state, glm::ivec2(pos.x + x, pos.y + z));
		}
	}
}


void initGame(GameState *state, unsigned int shaderID)
{
	state->mode = WORLD;
	initPlayer(&state->player);
	loadBlockData(&state->bDatabase, shaderID);
	for (int z = 0; z < WORLD_SIZE; z++)
	{
		for (int x = 0; x < WORLD_SIZE; x++)
		{
			glm::ivec2 positionToInit(x, z);
			generateChunk(state, positionToInit);
		}
	}
}

int main(int argc, char* argv[])
{
	int windowWidth = 1280;
	int windowHeight = 720;

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *window = SDL_CreateWindow("BlockWorld", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_GL_SetSwapInterval(0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GLContext glContext = SDL_GL_CreateContext(window);


	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);



	GLuint texturedShader = createShader("TexturedVertexShader.glsl", "TexturedFragmentShader.glsl");

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	

	GameState state = {};
	initGame(&state, texturedShader);

	bool firstrun = true;



	while (state.gameShouldRun)
	{
		state.currentTime = SDL_GetTicks() / 1000.0f;
		state.deltaTime = state.currentTime - state.lastTime;
		state.lastTime = state.currentTime;



		glm::vec3 playerPos = state.player.position;
		glm::ivec3 playerIntPos = glm::ivec3((float)playerPos.x, (float)playerPos.y, (float)playerPos.z);
		Chunk *playerChunk = findChunkAtWorldPos(&state, playerIntPos);
		glm::ivec2 playerChunkPos = playerToChunkPos(playerIntPos);
		glm::ivec3 playerIntPosInChunk = worldToChunkPosition(playerIntPos, playerChunkPos);



		SDL_Event windowEvent;
		while (SDL_PollEvent(&windowEvent))
		{
			switch (windowEvent.type)
			{
				case SDL_QUIT:
				{
					state.gameShouldRun = false;
				}break;
				case SDL_WINDOWEVENT:
				{
					if (windowEvent.window.event == SDL_WINDOWEVENT_RESIZED)
					{
						windowWidth  = windowEvent.window.data1;
						windowHeight = windowEvent.window.data2;
					}
				}break;
				case SDL_KEYDOWN:
				{
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					{
						state.gameShouldRun = false;
					}
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_L)
					{
						state.shouldGenerate = true;
						std::cout << state.shouldGenerate << std::endl;
					}
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_P)
					{
						state.shouldGenerate = false;
						std::cout << state.shouldGenerate << std::endl;
					}
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_T)
					{
						debugState.isWireFrame = !debugState.isWireFrame;
					}
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_1)
					{
						if (checkChunkExists(playerChunkPos, &state))
						{
							playerPlaceBlock(Grass, playerIntPosInChunk, playerChunk);
						}
					}
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_2)
					{
						if (checkChunkExists(playerChunkPos, &state))
						{
							playerPlaceBlock(Dirt, playerIntPosInChunk, playerChunk);
						}
					}
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_3)
					{
						if (checkChunkExists(playerChunkPos, &state))
						{
							playerPlaceBlock(Stone, playerIntPosInChunk, playerChunk);
						}
					}
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_4)
					{
						if (checkChunkExists(playerChunkPos, &state))
						{
							playerPlaceBlock(Bedrock, playerIntPosInChunk, playerChunk);
						}
					}
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_5)
					{
						generateChunk(&state, playerChunkPos);
					}
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_RETURN)
					{
						state.chunks.clear();
					}
				
				}break;
				case SDL_MOUSEBUTTONDOWN:
				{
					if (windowEvent.button.button == SDL_BUTTON_LEFT)
					{
						generateChunk(&state, playerChunkPos);
					}
				}break;
			}
		}

#if 1
		int xOffset, yOffset;
		SDL_GetRelativeMouseState(&xOffset, &yOffset);
		playerLook(&state.player, (float)xOffset, (float)yOffset);
		SDL_PumpEvents();
		const Uint8 *keyboardState = SDL_GetKeyboardState(0);
		if (keyboardState[SDL_SCANCODE_W])
			movePlayer(&state.player, FORWARD, state.deltaTime);
		if (keyboardState[SDL_SCANCODE_A])
			movePlayer(&state.player, LEFT, state.deltaTime);
		if (keyboardState[SDL_SCANCODE_S])
			movePlayer(&state.player, BACKWARD, state.deltaTime);
		if (keyboardState[SDL_SCANCODE_D])
			movePlayer(&state.player, RIGHT, state.deltaTime);
		if (keyboardState[SDL_SCANCODE_SPACE])
			movePlayer(&state.player, UP, state.deltaTime);
		if (keyboardState[SDL_SCANCODE_LSHIFT])
			movePlayer(&state.player, DOWN, state.deltaTime);

		updatePlayer(&state.player);
#endif // 0

		if (state.firstRun)
		{
			state.shouldGenerate = false;
			std::cout << "===========================================" << std::endl;
		}



		SDL_SetWindowTitle(window, std::string("X: " + std::to_string(playerPos.x) + " Y: " + std::to_string(playerPos.y) + " Z: " + std::to_string(playerPos.z) + " Chunks: " + std::to_string(debugState.nChunks) + " Blocks: " + std::to_string(debugState.nBlocks) + " FPS: " + std::to_string(1 / state.deltaTime)).c_str());

		glViewport(0, 0, windowWidth, windowHeight);
		glClearColor(9.0f / 255.0f, 64.0f / 255.0f, 255.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (debugState.isWireFrame)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		glUseProgram(texturedShader);
		glm::mat4 projection = glm::perspective(glm::radians(state.player.camera.fov), (float)windowWidth / (float)windowHeight, 0.1f, state.player.camera.viewDistance);
		glm::mat4 view = GetCameraViewMatrix(state.player.camera);
		setUniformMat4("projection", projection, texturedShader);
		setUniformMat4("view", view, texturedShader);

		int blockCounter = 0;
		int chunkCounter = 0;
		for (std::unordered_map<long long int, Chunk>::iterator iter = state.chunks.begin(); iter != state.chunks.end(); ++iter)
		{
			drawChunk((Chunk*)&iter->second,&state, texturedShader);
			blockCounter += (int)iter->second.blocks.size();
		}


		SDL_GL_SwapWindow(window);
		state.firstRun = false;
	}



	return 0;
}

