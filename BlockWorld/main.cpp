#include <SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <assimp\Importer.hpp>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Player.h"
#include "shader.h"
#include "camera.h"
#include "Model.h"


/*

float vertices[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};
*/
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};


/*
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};
*/


#define WORLD_BLOCKSIZE 1.0f
#define WORLD_WIDTH 4
#define WORLD_HEIGHT 4
#define WORLD_DEPTH 4

enum ToolRequired
{
	WOOD,
	STONE,
	IRON,
	GOLD,
	DIAMOND
};


enum BlockType
{
	Dirt,
	BirchLog
};


void loadBlockModels(ModelManager *modelManager)
{
	LoadModel("A:\\Programming\\C++\\Projects\\BlockWorld\\BlockWorld\\resources\\Cube\\CUBE.obj", modelManager, BirchLog);
}

struct Block
{
	float durability;	
	float luminance;
	ToolRequired toolRequired;
	glm::ivec3 position;

	BlockType blockType;
	bool isRightClickable = false;
};

struct Chunk
{
	glm::ivec3 position;
	int nBlocks;
	Block *blocks;
};

void initChunk(Chunk *chunk, ModelManager *modelManager)
{
	chunk->blocks = (Block*)calloc(36, sizeof(Block));
	chunk->nBlocks = 36;
	for (int z = 0; z < 6; z++)
	{
		for (int x = 0; x < 6; x++)
		{
			Block* block = ((chunk->blocks + z * 6) + x);
			block->position = chunk->position * glm::ivec3(x * 2, 1 * 2, z * 2);
			block->blockType = BirchLog;
		}
	}
}

void drawChunk(Chunk *chunk,GLuint shader, ModelManager *modelManager)
{
	for (int z = 0; z < 6; z++)
	{
		for (int x = 0; x < 6; x++)
		{
			Block* block = ((chunk->blocks + z * 6) + x);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(block->position));
			setUniformMat4("model", model, shader);
			Model *DModel = getModel(modelManager, block->blockType);
			DrawModel(shader, DModel);
		}
	}
}
struct gameWorld
{
	Chunk *chunks = new Chunk[WORLD_WIDTH * WORLD_HEIGHT * WORLD_DEPTH];
	int nChunks = WORLD_WIDTH * WORLD_HEIGHT * WORLD_DEPTH;
};

void initWorld(gameWorld *world, ModelManager *modelManager)
{
	for (int z = 0; z < 4; z++)
	{
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				Chunk *chunk = world->chunks + x + y * WORLD_WIDTH + WORLD_HEIGHT * WORLD_DEPTH * z;
				chunk->position = glm::ivec3(x, y, z);
				initChunk(chunk, modelManager);
				std::cout << "Chunk Init At: " << chunk->position.x << chunk->position.y << chunk->position.z << std::endl;
			}
		}
	}
}

void drawWorld(gameWorld *world, GLuint shader, ModelManager *modelManager)
{
	for (int z = 0; z < 4; z++)
	{
		for (int y = 0 ; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				Chunk *chunk = world->chunks + x + y * WORLD_WIDTH + WORLD_HEIGHT * WORLD_DEPTH * z;
				drawChunk(chunk, shader, modelManager);
			}
		}
	}
}

enum GAME_MODE
{
	MENU,
	WORLD
};

struct gameState
{
	GAME_MODE mode;
	Player player;
	bool gameShouldRun = true;
	bool firstCamera = true;
	float deltaTime = 0.0f;
	float lastTime = 0.0f;
	float currentTime = 0.0f;
};


void initGame(gameState *state)
{
	state->mode = WORLD;
	initPlayer(&state->player);
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


	GLuint modelShader = createShader("modelVertexShader.glsl", "modelFragmentShader.glsl");


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	


	gameState state;
	initGame(&state);


	ModelManager modelManager;
	loadBlockModels(&modelManager);
	
	//LoadModel("A:\\Programming\\C++\\Projects\\BlockWorld\\BlockWorld\\resources\\Cube\\CUBE.obj", &modelManager);

	gameWorld world;
	initWorld(&world, &modelManager);



	while (state.gameShouldRun)
	{
	//	std::cout << "Start Frame " << frame++ << std::endl;
		state.currentTime = SDL_GetTicks() / 1000.0f;
		state.deltaTime = state.currentTime - state.lastTime;
		state.lastTime = state.currentTime;

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
						state.gameShouldRun = false;
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


		glViewport(0, 0, windowWidth, windowHeight);
		glClearColor(9.0f / 255.0f, 64.0f / 255.0f, 255.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	


		glUseProgram(modelShader);

		glm::mat4 projection = glm::perspective(glm::radians(state.player.camera.fov), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
		glm::mat4 view = GetCameraViewMatrix(state.player.camera);
		setUniformMat4("projection", projection, modelShader);
		setUniformMat4("view", view, modelShader);

		drawWorld(&world, modelShader, &modelManager);

		SDL_GL_SwapWindow(window);
	}

//	delete [] world.blocks;

	return 0;
}

