#include <SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <vector>
#include <assimp\Importer.hpp>
#include <stdio.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "Model.h"




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

glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};


int main(int argc, char* argv[])
{
	bool gameShouldRun = true;
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
	



	Camera camera;
	camera.position = glm::vec3(0.0f, 0.0f, 5.0f);
	camera.front = glm::vec3(0.0f, 0.0f, 1.0f);
	camera.up = glm::vec3(0.0f, 1.0f, 0.0f);
	camera.moveSpeed = SPEED;
	camera.mouseSensitivity = SENSITIVTY;
	camera.fov = ZOOM;
	camera.yaw = YAW;
	camera.pitch = PITCH;
	camera.worldUp = camera.up;
	bool firstCamera = true;
	UpdateCameraVectors(&camera);

	glm::vec3 lightPos = glm::vec3(1.2f, 1.0f, 2.0f);

	float deltaTime = 0.0f;
	float lastTime = 0.0f;
	float currentTime = 0.0f;


	int frame = 0;
	Model hand;
	LoadModel("A:\\Programming\\C++\\Projects\\BlockWorld\\BlockWorld\\resources\\Hand\\hand.OBJ", &hand);
	Model nanosuit;
	LoadModel("A:\\Programming\\C++\\Projects\\BlockWorld\\BlockWorld\\resources\\Nanosuit\\nanosuit.obj", &nanosuit);




	while (gameShouldRun)
	{
	//	std::cout << "Start Frame " << frame++ << std::endl;
		currentTime = SDL_GetTicks() / 1000.0f;
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		SDL_Event windowEvent;
		while (SDL_PollEvent(&windowEvent))
		{
			switch (windowEvent.type)
			{
				case SDL_QUIT:
				{
					gameShouldRun = false;
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
						gameShouldRun = false;
				}break;
			}
		}

#if 1
		int xOffset, yOffset;
		SDL_GetRelativeMouseState(&xOffset, &yOffset);
		CameraLook(&camera, (float)xOffset, (float)yOffset, true);
		SDL_PumpEvents();
		const Uint8 *keyboardState = SDL_GetKeyboardState(0);
		if (keyboardState[SDL_SCANCODE_W])
			MoveCamera(&camera, FORWARD, deltaTime);
		if (keyboardState[SDL_SCANCODE_A])
		MoveCamera(&camera, LEFT, deltaTime);
		if (keyboardState[SDL_SCANCODE_S])
			MoveCamera(&camera, BACKWARD, deltaTime);
		if (keyboardState[SDL_SCANCODE_D])
			MoveCamera(&camera, RIGHT, deltaTime);
		if (keyboardState[SDL_SCANCODE_SPACE])
			MoveCamera(&camera, UP, deltaTime);
		if (keyboardState[SDL_SCANCODE_LSHIFT])
			MoveCamera(&camera, DOWN, deltaTime);
#endif // 0


		glViewport(0, 0, windowWidth, windowHeight);
		glClearColor(50.0f / 255.0f, 32.5f / 255.0f, 82.5f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		



		glUseProgram(modelShader);

		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
		glm::mat4 view = GetCameraViewMatrix(camera);
		setUniformMat4("projection", projection, modelShader);
		setUniformMat4("view", view, modelShader);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		setUniformMat4("model", model, modelShader);
		DrawModel(modelShader, &nanosuit);

			


		SDL_GL_SwapWindow(window);
	}



	return 0;
}

