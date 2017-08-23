#include <SDL.h>
#include <GL\glew.h>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "camera.h"


char* file_read(const char* filename) {
	SDL_RWops *rw = SDL_RWFromFile(filename, "rb");
	if (rw == NULL) return NULL;

	Sint64 res_size = SDL_RWsize(rw);
	char* res = (char*)malloc(res_size + 1);

	Sint64 nb_read_total = 0, nb_read = 1;
	char* buf = res;
	while (nb_read_total < res_size && nb_read != 0) {
		nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
		nb_read_total += nb_read;
		buf += nb_read;
	}
	SDL_RWclose(rw);
	if (nb_read_total != res_size) {
		free(res);
		return NULL;
	}

	res[nb_read_total] = '\0';
	return res;
}

void setUniformMat4(char* uniformName, glm::mat4 matrix, GLuint program)
{
	int location = glGetUniformLocation(program, uniformName);
	glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}
void setUniformVec3(char* uniformName, glm::vec3 vector, GLuint program)
{
	int location = glGetUniformLocation(program, uniformName);
	glUniform3f(location, vector.x, vector.y, vector.z);
}
void setUniformFloat(char* uniformName, float f, GLuint program)
{
	int location = glGetUniformLocation(program, uniformName);
	glUniform1f(location, f);
}
float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
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

unsigned int indices[] = { 0, 1, 2 , 1, 2, 3};



int main(int argc, char* argv[])
{
	int counter = 0;

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


	char* vShader = file_read("vertexShader.glsl");
	char* fShader = file_read("fragmentShader.glsl");

	char* lVShader = file_read("lightVertexShader.glsl");
	char* lFShader = file_read("lightFragShader.glsl");

	char infoLog[512];
	int success;
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShader, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShader, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLuint lightVertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(lightVertexShader, 1, &lVShader, NULL);
	glCompileShader(lightVertexShader);
	if (!success)
	{
		glGetShaderInfoLog(lightVertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LIGHTVERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	GLuint lightFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(lightFragmentShader, 1, &lFShader, NULL);
	glCompileShader(lightFragmentShader);
	if (!success)
	{
		glGetShaderInfoLog(lightVertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LIGHTFRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	GLuint lightShaderProgram = glCreateProgram();
	glAttachShader(lightShaderProgram, lightVertexShader);
	glAttachShader(lightShaderProgram, lightFragmentShader);
	glLinkProgram(lightShaderProgram);
	glDeleteShader(lightVertexShader);
	glDeleteShader(lightFragmentShader);

	
	GLuint vao;
	GLuint vbo;

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);


	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);


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
	glm::vec3 lightColor;

	float deltaTime = 0.0f;
	float lastTime = 0.0f;
	float currentTime = 0.0f;


	int frame = 0;
	while (gameShouldRun)
	{
	//	std::cout << "Start Frame " << frame++ << std::endl;
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		currentTime = SDL_GetTicks() / 1000.0f;



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
		glClearColor(100.0f / 255.0f, 65.0f / 255.0f, 165.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		lightColor.x = sinf(currentTime * 2.0f);
		lightColor.y = sinf(currentTime * 0.7f);
		lightColor.z = sinf(currentTime * 1.3f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

		glUseProgram(shaderProgram);
		setUniformVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f), shaderProgram);
		setUniformVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f), shaderProgram);
		setUniformVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f), shaderProgram);
		setUniformVec3("light.ambient", ambientColor, shaderProgram);
		setUniformVec3("light.diffuse", diffuseColor, shaderProgram);
		setUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f), shaderProgram);
		setUniformVec3("light.position", lightPos, shaderProgram);
		setUniformFloat("material.shininess", 32.0f, shaderProgram);

		setUniformVec3("viewPos", camera.position, shaderProgram);
		setUniformVec3("objectColor", glm::vec3(1.0f, 0.2f, 0.3f), shaderProgram);

		glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
		setUniformMat4("projection", projection, shaderProgram);
		glm::mat4 view = GetCameraViewMatrix(camera);
		setUniformMat4("view", view, shaderProgram);

		for (int i = 0; i < 10; i++)
		{
			float angle = 20.0f * i * (SDL_GetTicks() / 100000.0f);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(cubePositions[i]));
			model = glm::rotate(model, angle, glm::vec3((float)i * 0.5f, (float)i * 0.34, (float)i));
			setUniformMat4("model", model, shaderProgram);
			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glUseProgram(lightShaderProgram);
		glBindVertexArray(lightVAO);
		setUniformMat4("projection", projection, lightShaderProgram);
		setUniformMat4("view", view, lightShaderProgram);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.5f));
		setUniformMat4("model", model, lightShaderProgram);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		SDL_GL_SwapWindow(window);
	}



	return 0;
}