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
#include "Ray.h"
#include "GameState.h"
#include "shader.h"
#include "camera.h"






void placeTree(glm::ivec3 position, Chunk* chunk)
{
	int x = position.x;
	int y = position.y;
	int z = position.z;

	for (int i = 1; i <= 6; i++)
	{
		placeBlock(Stone, glm::ivec3(x, y + i, z), chunk);
	}
	for (int i = 4; i <= 5; i++)
	{
		placeBlock(Bedrock, glm::ivec3(x - 2, y + i, z - 2), chunk);
		placeBlock(Bedrock, glm::ivec3(x - 1, y + i, z - 2), chunk);
		placeBlock(Bedrock, glm::ivec3(x    , y + i, z - 2), chunk);
		placeBlock(Bedrock, glm::ivec3(x + 1, y + i, z - 2), chunk);
		placeBlock(Bedrock, glm::ivec3(x + 2, y + i, z - 2), chunk);

		placeBlock(Bedrock, glm::ivec3(x - 2, y + i, z - 1), chunk);
		placeBlock(Bedrock, glm::ivec3(x - 1, y + i, z - 1), chunk);
		placeBlock(Bedrock, glm::ivec3(x    , y + i, z - 1), chunk);
		placeBlock(Bedrock, glm::ivec3(x + 1, y + i, z - 1), chunk);
		placeBlock(Bedrock, glm::ivec3(x + 2, y + i, z - 1), chunk);

		placeBlock(Bedrock, glm::ivec3(x - 2, y + i, z    ), chunk);
		placeBlock(Bedrock, glm::ivec3(x - 1, y + i, z    ), chunk);
		placeBlock(Bedrock, glm::ivec3(x + 1, y + i, z    ), chunk);
		placeBlock(Bedrock, glm::ivec3(x + 2, y + i, z    ), chunk);

		placeBlock(Bedrock, glm::ivec3(x - 2, y + i, z + 1), chunk);
		placeBlock(Bedrock, glm::ivec3(x - 1, y + i, z + 1), chunk);
		placeBlock(Bedrock, glm::ivec3(x    , y + i, z + 1), chunk);
		placeBlock(Bedrock, glm::ivec3(x + 1, y + i, z + 1), chunk);
		placeBlock(Bedrock, glm::ivec3(x + 2, y + i, z + 1), chunk);

		placeBlock(Bedrock, glm::ivec3(x - 2, y + i, z + 2), chunk);
		placeBlock(Bedrock, glm::ivec3(x - 1, y + i, z + 2), chunk);
		placeBlock(Bedrock, glm::ivec3(x    , y + i, z + 2), chunk);
		placeBlock(Bedrock, glm::ivec3(x + 1, y + i, z + 2), chunk);
		placeBlock(Bedrock, glm::ivec3(x + 2, y + i, z + 2), chunk);
	}
	for (int i = 6; i <= 7; i++)
	{
		placeBlock(Bedrock, glm::ivec3(x - 1, y + i, z), chunk);
		placeBlock(Bedrock, glm::ivec3(x + 1, y + i, z    ), chunk);
		placeBlock(Bedrock, glm::ivec3(x    , y + i, z - 1), chunk);
		placeBlock(Bedrock, glm::ivec3(x    , y + i, z + 1), chunk);
	}
	placeBlock(Bedrock, glm::ivec3(x, y + 7, z), chunk);
	generateChunkMesh(chunk);
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
				int height = (int)scaled_octave_noise_2d(5.0f, 7.0f, 0.0007f, 16.0f, 48.0f, (float)(chunkBottomLeft.x * CHUNK_SIZE + x), (float)(chunkBottomLeft.z * CHUNK_SIZE + z));
				//float shouldPlace = scaled_octave_noise_3d(4.0f, 2.0f, 0.007f, -1.0f, 1.0f, (float)(chunkBottomLeft.x * CHUNK_SIZE + x),(float)(chunkBottomLeft.y + y), (float)(chunkBottomLeft.z * CHUNK_SIZE + z));

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
	for (int z = -size; z < size; z++)
	{
		for (int x = -size; x < size; x++)
		{
			glm::ivec2 posToGen = { pos.x + x, pos.y + z };
			if (!checkChunkExists(posToGen, state))
				generateChunk(state, posToGen);
		}
	}
}


void initGame(GameState *state)
{
	state->mode = WORLD;
	initPlayer(&state->player);
	RawModel cubeModel;
	std::vector<float> cubeUVs;
	for (int i = 0; i < 6; i++) 
	{
		addUVs(Selector, Top, &cubeUVs);
	}

	cubeModel = loadToVAO(cubeVertices, ARRAY_COUNT(cubeVertices), &cubeUVs[0], (unsigned int)cubeUVs.size());
	state->cubeModel = cubeModel;

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

	

	GLuint lightedShader = createShader("LightedVertexShader.glsl", "LightedFragmentShader.glsl");
	GLuint texturedShader = createShader("TexturedVertexShader.glsl", "TexturedFragmentShader.glsl");



	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	

	GameState state = {};
	initGame(&state);

	bool firstrun = true;


	while (state.gameShouldRun)
	{
		state.currentTime = SDL_GetTicks() / 1000.0f;
		state.deltaTime = state.currentTime - state.lastTime;
		state.lastTime = state.currentTime;



		glm::ivec3 playerIntPos = glm::trunc(state.player.position);
		Chunk *playerChunk = findChunkAtWorldPos(&state, playerIntPos);
		glm::ivec2 playerChunkPos = posOfChunkAtWorldPos(playerIntPos);
		glm::ivec3 playerIntPosInChunk = worldToChunkPosition(playerIntPos, playerChunkPos);
		state.focusedBlockPos = glm::trunc(findFirstBlock(0.001f, state.player.pickDistance, &state.playerRay, &state));


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
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_LCTRL)
					{
						state.goFast = !state.goFast;
					}
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_T)
					{
						debugState.isWireFrame = !debugState.isWireFrame;
					}
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_1)
					{
						state.blockTypeToPlace = Grass;
					}
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_2)
					{
						state.blockTypeToPlace = Dirt;
					}
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_3)
					{
						state.blockTypeToPlace = Stone;
					}
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_4)
					{
						state.blockTypeToPlace = Bedrock;
					}
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_5)
					{
						state.blockTypeToPlace = WoodPlank;
					}
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_6)
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
						Chunk *chunkWithBlockToRemove = findChunkAtWorldPos(&state, state.focusedBlockPos);
						removeBlock(glm::trunc(state.focusedBlockPos), chunkWithBlockToRemove);
					}
					if (windowEvent.button.button == SDL_BUTTON_RIGHT)
					{
						state.focusedFacePos = findLastSpace(0.001f, state.player.pickDistance, &state.playerRay, &state);
						Chunk *chunkWithBlockToPlace = findChunkAtWorldPos(&state, state.focusedFacePos);
						playerPlaceBlock(state.blockTypeToPlace, glm::ivec3(state.focusedFacePos.x, state.focusedFacePos.y, state.focusedFacePos.z), chunkWithBlockToPlace);
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


		getRayDirection(state.player.camera, glm::vec2(windowWidth / 2.0f, windowHeight / 2.0f), glm::vec2(windowWidth, windowHeight));
		initRay(&state.playerRay, state.player.camera.position, state.player.camera.front);
#endif // 0

		if (state.firstRun)
		{
			state.shouldGenerate = false;
			std::cout << "===========================================" << std::endl;
		}


		SDL_SetWindowTitle(window, std::string("X: " + std::to_string(state.player.position.x) + " Y: " + std::to_string(state.player.position.y) + " Z: " + std::to_string(state.player.position.z) + " Chunks: " + std::to_string(debugState.nChunks) + " Blocks: " + std::to_string(debugState.nBlocks) + " FPS: " + std::to_string(1 / state.deltaTime)).c_str());

		glViewport(0, 0, windowWidth, windowHeight);
		glClearColor(9.0f / 255.0f, 64.0f / 255.0f, 255.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		if (debugState.isWireFrame)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		if (state.goFast)
			state.player.moveSpeed = 50.0f;
		else
			state.player.moveSpeed = 10.0f;


		glDisable(GL_BLEND);

		glUseProgram(lightedShader);
		glm::mat4 projection = glm::perspective(glm::radians(state.player.camera.fov), (float)windowWidth / (float)windowHeight, 0.1f, state.player.camera.viewDistance);
		glm::mat4 view = GetCameraViewMatrix(state.player.camera);
		setUniformMat4("projection", projection, lightedShader);
		setUniformMat4("view", view, lightedShader);


		int blockCounter = 0;
		int chunkCounter = 0;
		for (std::unordered_map<long long int, Chunk>::iterator iter = state.chunks.begin(); iter != state.chunks.end(); ++iter)
		{
			drawChunk((Chunk*)&iter->second,&state, lightedShader);
			blockCounter += (int)iter->second.blocks.size();
		}

		glUseProgram(texturedShader);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(state.focusedBlockPos));
		model = glm::scale(model, glm::vec3(1.0025f, 1.0025f, 1.0025f));
		model = glm::translate(model, glm::vec3(-0.00125f, -0.00125f, -0.00125f));
		setUniformMat4("model", model, texturedShader);
		setUniformMat4("view", view, lightedShader);
		setUniformMat4("projection", projection, lightedShader);
		drawModel(state.cubeModel, state.atlas);

		{

		
		glClear(GL_DEPTH_BUFFER_BIT);

		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);

		glUseProgram(0);

		float vertices[] = 
		{
			-0.005f, -0.01f, 0.0f,
			 0.005f, -0.01f, 0.0f,
			 0.00f,  0.01f, 0.0f
		};

		RawModel ui;
		ui = loadToVAO(vertices, ARRAY_COUNT(vertices), vertices, 3);


		drawModel(ui);

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		}

		


		SDL_GL_SwapWindow(window);
		state.firstRun = false;
	}



	return 0;
}

