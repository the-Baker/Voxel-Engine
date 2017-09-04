#include <SDL.h>
#include <SDL_thread.h>
#include <GL\glew.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <thread>
#include <assimp\Importer.hpp>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Utility.h"

#include "Player.h"
#include "Model.h"
#include "Block.h"
#include "shader.h"
#include "camera.h"
#include "noise.h"




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
	bool firstCamera = true;
	float deltaTime = 0.0f;
	float lastTime = 0.0f;
	float currentTime = 0.0f;
	std::vector<Block> blocks;
	std::vector<glm::ivec3> usedPositions;
	BlockDatabase bDatabase;
};

void initGame(GameState *state, unsigned int shaderID)
{
	state->mode = WORLD;
	initPlayer(&state->player);
	loadBlockData(&state->bDatabase, shaderID);
}

static int generateWorld(void *state, glm::ivec3 playerBlockPos)
{
	GameState *gamestate = (GameState*)state;


	if (gamestate->shouldGenerate)
	{
		for (int z = playerBlockPos.z - GENERATE_HALF_SIZE; z < playerBlockPos.z + GENERATE_HALF_SIZE; z++)
		{
			for (int y = 0; y < 1; y++)
			{
				for (int x = playerBlockPos.x - GENERATE_HALF_SIZE; x < playerBlockPos.x + GENERATE_HALF_SIZE; x++)
				{
					int height = (int)(noise((float)x / 7.0f, (float)z / 7.0f) * 3.0 + 16);
					for (int i = 0; i <= height; i++)
					{
						if (i > height - 1)
							placeBlock(Grass, glm::ivec3(x, i, z), &gamestate->blocks, &gamestate->usedPositions);
						else if (i > height - 6)
							placeBlock(Dirt, glm::ivec3(x, i, z), &gamestate->blocks, &gamestate->usedPositions);
						else if (i == 0)
							placeBlock(Bedrock, glm::ivec3(x, i, z), &gamestate->blocks, &gamestate->usedPositions);
						else
							placeBlock(StoneBrick, glm::ivec3(x, i, z), &gamestate->blocks, &gamestate->usedPositions);
					}
				}
			}
		}
	}

	return 1;
}

int main(int argc, char* argv[])
{
	int windowWidth = 1280;
	int windowHeight = 720;

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *window = SDL_CreateWindow("BlockWorld", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GLContext glContext = SDL_GL_CreateContext(window);


	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);


	//GLuint modelShader = createShader("CubeInstanceVertexShader.glsl", "CubeInstanceFragmentShader.glsl");
	GLuint texturedShader = createShader("TexturedVertexShader.glsl", "TexturedFragmentShader.glsl");

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	


	GameState state;
	initGame(&state, texturedShader);



	bool firstrun = true;



	while (state.gameShouldRun)
	{
		state.currentTime = SDL_GetTicks() / 1000.0f;
		state.deltaTime = state.currentTime - state.lastTime;
		state.lastTime = state.currentTime;


		//std::cout << state.deltaTime << std::endl;
		glm::vec3 playerPos = state.player.position;
		glm::ivec3 playerBlockPos = glm::ivec3(roundFloatToInt(playerPos.x), roundFloatToInt(playerPos.y), roundFloatToInt(playerPos.z));

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
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_P)
					{
						state.shouldGenerate = false;
						std::cout << state.shouldGenerate << std::endl;
					}
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_L)
					{
						state.shouldGenerate = true;
						std::cout << state.shouldGenerate << std::endl;
					}
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_1)
					{
						placeBlock(Grass, playerBlockPos, &state.blocks, &state.usedPositions);
						std::cout << playerBlockPos.x << " " << playerBlockPos.y << " " << playerBlockPos.z << std::endl;
						std::cout << playerPos.x << " " << playerPos.y << " " << playerPos.z << std::endl;
					}
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_2)
					{
						placeBlock(Dirt, playerBlockPos, &state.blocks, &state.usedPositions);
					}
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_3)
					{
						placeBlock(StoneBrick, playerBlockPos, &state.blocks, &state.usedPositions);
					}
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_4)
					{
						placeBlock(Bedrock, playerBlockPos, &state.blocks, &state.usedPositions);
					}
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_RETURN)
					{
						state.blocks.clear();
						state.usedPositions.clear();
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

		generateWorld(&state, playerBlockPos);


		glViewport(0, 0, windowWidth, windowHeight);
		glClearColor(9.0f / 255.0f, 64.0f / 255.0f, 255.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(texturedShader);
		glm::mat4 projection = glm::perspective(glm::radians(state.player.camera.fov), (float)windowWidth / (float)windowHeight, 0.1f, state.player.camera.viewDistance);
		glm::mat4 view = GetCameraViewMatrix(state.player.camera);
		setUniformMat4("projection", projection, texturedShader);
		setUniformMat4("view", view, texturedShader);

		
		for (int i = 0; i < state.blocks.size(); i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, (glm::vec3)state.usedPositions[i]);
			setUniformMat4("model", model, texturedShader);
			drawBlock(state.blocks[i], &state.bDatabase);
		}
		

		SDL_GL_SwapWindow(window);
	}



	return 0;
}

