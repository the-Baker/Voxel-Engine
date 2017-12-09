#include <SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <unordered_map>
#include <stdio.h>
#include <string>
#include <math.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Utility.h"
#include "simplexnoise.h"

#include "Physics.h"
#include "Player.h"
#include "Model.h"
#include "Block.h"
#include "Chunk.h"
#include "Ray.h"
#include "GameState.h"
#include "shader.h"
#include "camera.h"



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
				//height = 1;
				for (int i = 0; i < height; i++)
				{
					glm::ivec3 positionToPlace = glm::ivec3(x, i, z);

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

void generateWorldAroundPosition(GameState *state, glm::ivec2 pos, int halfSize)
{
	for (int z = -halfSize; z < halfSize; z++)
	{
		for (int x = -halfSize; x < halfSize; x++)
		{
			glm::ivec2 posToGen = { pos.x + x, pos.y + z };
			if (!checkChunkExists(posToGen, state))
				generateChunk(state, posToGen);
		}
	}
}

void destroyDistantWorld(GameState *state, glm::ivec2 pos, int halfSize)
{
	glm::ivec2 chunkPos;
	for (auto iterator = state->chunks.begin(); iterator != state->chunks.end(); iterator++)
	{
		chunkPos = iterator->second.position;
		if (chunkPos.x < pos.x - halfSize || chunkPos.x > pos.x + halfSize || chunkPos.y < pos.y - halfSize || chunkPos.y > pos.y + halfSize)
			state->chunks.erase(iterator->first);
	}
}

bool blockExists(glm::vec3 position, GameState *state)
{
	Chunk *chunk = findChunkAtWorldPos(state, position);
	return (chunk->blocks.count(vec3ToInt(worldToChunkPosition(position, chunk->position))) > 0);
}

bool blockExists(glm::vec3 position, Chunk* chunk)
{
	return (chunk->blocks.count(vec3ToInt(worldToChunkPosition(position, chunk->position))) > 0);
}

bool isHitboxInBlock(glm::vec3 position, glm::vec3 hitBoxDimensions, GameState *state)
{
	glm::vec3 playerBottomLeftBack =   { position.x - hitBoxDimensions.x * 0.5f, position.y,  position.z - hitBoxDimensions.z * 0.5f };
	glm::vec3 playerBottomRightBack =  { position.x + hitBoxDimensions.x * 0.5f, position.y,  position.z - hitBoxDimensions.z * 0.5f };
	glm::vec3 playerBottomLeftFront =  { position.x - hitBoxDimensions.x * 0.5f, position.y,  position.z + hitBoxDimensions.z * 0.5f };
	glm::vec3 playerBottomRightFront = { position.x + hitBoxDimensions.x * 0.5f, position.y,  position.z + hitBoxDimensions.z * 0.5f };
	glm::vec3 playerTopLeftBack =      { position.x - hitBoxDimensions.x * 0.5f, position.y + hitBoxDimensions.y, position.z - hitBoxDimensions.z * 0.5f };
	glm::vec3 playerTopRightBack =     { position.x + hitBoxDimensions.x * 0.5f, position.y + hitBoxDimensions.y, position.z - hitBoxDimensions.z * 0.5f };
	glm::vec3 playerTopLeftFront =     { position.x - hitBoxDimensions.x * 0.5f, position.y + hitBoxDimensions.y, position.z + hitBoxDimensions.z * 0.5f };
	glm::vec3 playerTopRightFront =    { position.x + hitBoxDimensions.x * 0.5f, position.y + hitBoxDimensions.y, position.z + hitBoxDimensions.z * 0.5f };

	return (blockExists(playerBottomLeftBack, state) || blockExists(playerBottomRightBack, state) || blockExists(playerBottomLeftFront, state)
		|| blockExists(playerBottomRightFront, state) || blockExists(playerTopLeftBack, state) || blockExists(playerTopRightBack, state)
		|| blockExists(playerTopLeftFront, state) || blockExists(playerTopRightFront, state));
}

void movePlayer(Player *player, GameState *state)
{
	player->acceleration = glm::vec3(0.0f, GRAVITY, 0.0f);
	float velocityMagnitude = glm::length(player->velocity);
	float oppositePercent = 0.0f;
	float reactivityPercentage = 0.5f;

	float moveSpeed = player->moveSpeed;
	//float accelCoefficient = player->moveSpeed * state->deltaTime;
	glm::vec3 front = glm::normalize(player->front * glm::vec3(1.0f, 0.0f, 1.0f));
	glm::vec3 right = glm::normalize(player->right * glm::vec3(1.0f, 0.0f, 1.0f));


	Chunk *chunk = findChunkAtWorldPos(state, player->position);

	if (state->KeyboardState[SDL_SCANCODE_W])
	{
		player->acceleration += front * moveSpeed;
	}

	if (state->KeyboardState[SDL_SCANCODE_S])
	{
		player->acceleration -= front * moveSpeed;
	}

	if (state->KeyboardState[SDL_SCANCODE_A])
	{
		player->acceleration -= right * moveSpeed;
	}

	if (state->KeyboardState[SDL_SCANCODE_D])
	{
		player->acceleration += right * moveSpeed;
	}

	if (state->KeyboardState[SDL_SCANCODE_SPACE] && player->onGround)
	{
		player->velocity += player->worldUp * JUMPVALUE;
	}

	if (state->KeyboardState[SDL_SCANCODE_LSHIFT])
	{
		player->acceleration -= player->worldUp * moveSpeed;
	}
	

	glm::vec2 horizontalVelocity = glm::vec2(player->velocity.x, player->velocity.z);
	float horizontalVelocityMagnitude = glm::length(horizontalVelocity);
	if (horizontalVelocityMagnitude > player->maxSpeed)
	{
		player->velocity.x = glm::normalize(horizontalVelocity).x * player->maxSpeed;
		player->velocity.z = glm::normalize(horizontalVelocity).y * player->maxSpeed;
	}

	
	//Do Ground Friction
	if (player->onGround)
	{
		if (velocityMagnitude > 0.0f)
		{
			player->acceleration.x += player->velocity.x / velocityMagnitude * -FRICTION;
			player->acceleration.z += player->velocity.z / velocityMagnitude * -FRICTION;
		}
		if (horizontalVelocityMagnitude < 0.2f)
		{
			player->velocity.x = 0.0f;
			player->velocity.z = 0.0f;
		}
	}
	else
	{
		if (velocityMagnitude > 0.0f)
		{
			player->acceleration.x += player->velocity.x / velocityMagnitude * -AIR_RESISTANCE;
			player->acceleration.z += player->velocity.z / velocityMagnitude * -AIR_RESISTANCE;
		}
		if (horizontalVelocityMagnitude < 0.2f)
		{
			player->velocity.x = 0.0f;
			player->velocity.z = 0.0f;
		}
	}

	player->velocity += player->acceleration * state->deltaTime;


#define MOVEMENTSTEPS 16

	float incrementRatio = 1.0f / (float)MOVEMENTSTEPS;

	player->onGround = false;
	for (int i = 0; i < MOVEMENTSTEPS; i++)
	{
		glm::vec3 oldPosition = player->position;
		player->position.x += player->velocity.x * state->deltaTime * incrementRatio;
		if (isHitboxInBlock(player->position, player->hitBoxDimensions, state))
		{
			player->position.x = oldPosition.x;
			player->velocity.x = 0.0f;
		}
		player->position.y += player->velocity.y * state->deltaTime * incrementRatio;

		if (isHitboxInBlock(player->position, player->hitBoxDimensions, state))
		{
			player->position.y = oldPosition.y;
			player->velocity.y = 0.0f;
			player->onGround = true;
		}
		player->position.z += player->velocity.z * state->deltaTime * incrementRatio;
		if (isHitboxInBlock(player->position, player->hitBoxDimensions, state))
		{
			player->position.z = oldPosition.z;
			player->velocity.z = 0.0f;
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

	generateWorldAroundPosition(state, posOfChunkAtWorldPos(state->player.position), 4);
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
						state.atlas = loadTexture("MinecraftAdapted.png");
					}
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_P)
					{
						state.atlas = loadTexture("terrain.png");
					}
					if (windowEvent.key.keysym.scancode == SDL_SCANCODE_LCTRL)
					{
						state.goFast = !state.goFast;
						state.player.canFly = !state.player.canFly;
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

		glm::vec3 oldPlayerPos = state.player.position;

		int xOffset, yOffset;
		SDL_GetRelativeMouseState(&xOffset, &yOffset);
		playerLook(&state.player, (float)xOffset, (float)yOffset);
		SDL_PumpEvents();
		state.KeyboardState = SDL_GetKeyboardState(0);
		movePlayer(&state.player, &state);





		updatePlayer(&state.player);
		destroyDistantWorld(&state, playerChunkPos, 4);
		generateWorldAroundPosition(&state, playerChunkPos, 4);

		getRayDirection(state.player.camera, glm::vec2(windowWidth / 2.0f, windowHeight / 2.0f), glm::vec2(windowWidth, windowHeight));
		initRay(&state.playerRay, state.player.camera.position, state.player.camera.front);


		

		SDL_SetWindowTitle(window, std::string("X: " + std::to_string(state.player.position.x) + 
			" Y: " + std::to_string(state.player.position.y) + " Z: " + std::to_string(state.player.position.z) +
			" FT: " + std::to_string(state.deltaTime) + " FPS: " + std::to_string(1 / state.deltaTime) + 
			" FLYMODE: " + std::to_string(state.goFast) + "VX: " + std::to_string(state.player.velocity.x) 
			+ " VY: " + std::to_string(state.player.velocity.y) + " VZ: " + std::to_string(state.player.velocity.z)).c_str());

		glViewport(0, 0, windowWidth, windowHeight);
		glClearColor(9.0f / 255.0f, 64.0f / 255.0f, 255.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		if (debugState.isWireFrame)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);




		glDisable(GL_BLEND);

		glUseProgram(lightedShader);
		glm::mat4 projection = glm::perspective(glm::radians(state.player.camera.fov), (float)windowWidth / (float)windowHeight, 0.1f, state.player.camera.viewDistance);
		glm::mat4 view = GetCameraViewMatrix(state.player.camera);
		setUniformMat4("projection", projection, lightedShader);
		setUniformMat4("view", view, lightedShader);

		for (std::unordered_map<long long int, Chunk>::iterator iter = state.chunks.begin(); iter != state.chunks.end(); ++iter)
		{
			drawChunk((Chunk*)&iter->second,&state, lightedShader);
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
	}



	return 0;
}

