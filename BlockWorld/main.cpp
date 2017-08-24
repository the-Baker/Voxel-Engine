#include <SDL.h>
#include <GL\glew.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "shader.h"
#include "camera.h"


unsigned int loadTexture(char const* filename);

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


int main(int argc, char* argv[])
{
	bool gameShouldRun = true;
	int windowWidth = 1920;
	int windowHeight = 1080;

	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window *window = SDL_CreateWindow("BlockWorld", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GLContext glContext = SDL_GL_CreateContext(window);


	glewInit();
	glEnable(GL_DEPTH_TEST);



	GLuint shaderProgram = createShader("vertexShader.glsl", "spotLightFragShader.glsl");

	GLuint lightShaderProgram = createShader("lightVertexShader.glsl", "lightFragShader.glsl");

	GLuint vao;
	GLuint vbo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);


	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


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

	GLuint diffuseMap = loadTexture("container2.png");
	GLuint specularMap = loadTexture("container2_specular2.png");

	glUseProgram(shaderProgram);
	setUniformInt("material.diffuse", 0, shaderProgram);
	setUniformInt("material.specular", 1, shaderProgram);



	int frame = 0;
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


		glUseProgram(shaderProgram);
		setUniformVec3("light.position", camera.position, shaderProgram);
		setUniformVec3("light.direction", camera.front, shaderProgram);
		setUniformFloat("light.cutOff", glm::cos(glm::radians(12.5f)), shaderProgram);
		setUniformFloat("light.outerCutoff", glm::cos(glm::radians(17.5f)), shaderProgram);
		setUniformVec3("viewPos", camera.position, shaderProgram);


		setUniformVec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f), shaderProgram);
		setUniformVec3("light.diffuse", glm::vec3(0.8f, 0.8f, 0.8f), shaderProgram);
		setUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f), shaderProgram);
		setUniformFloat("light.constant", 1.0f, shaderProgram);
		setUniformFloat("light.linear", 0.09f, shaderProgram);
		setUniformFloat("light.quadratic", 0.032f, shaderProgram);

		setUniformFloat("material.shininess", 32.0f, shaderProgram);

		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
		glm::mat4 view = GetCameraViewMatrix(camera);
		setUniformMat4("projection", projection, shaderProgram);
		setUniformMat4("view", view, shaderProgram);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		glBindVertexArray(vao);
		for (int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			setUniformMat4("model", model, shaderProgram);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		/*
		glUseProgram(lightShaderProgram);
		setUniformMat4("projection", projection, lightShaderProgram);
		setUniformMat4("view", view, lightShaderProgram);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		setUniformMat4("model", model, lightShaderProgram);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		*/

		SDL_GL_SwapWindow(window);
	}



	return 0;
}

unsigned int loadTexture(char const *filename)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;

	unsigned char * data = stbi_load(filename, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "FAILURE: FAILED TO LOAD TEXTURE" << filename << std::endl;
		stbi_image_free(data);
	}
	return textureID;
}

