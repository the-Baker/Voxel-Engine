#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVTY = 0.3f;
const float ZOOM = 45.0f;

struct Camera
{
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	float moveSpeed;
	float mouseSensitivity;
	float fov;
};

glm::mat4 GetCameraViewMatrix(Camera camera)
{
	return glm::lookAt(camera.position, camera.position + camera.front, camera.up);
}

void UpdateCameraVectors(Camera *camera)
{
	glm::vec3 front;
	front.x = cos(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
	front.y = sin(glm::radians(camera->pitch));
	front.z = sin(glm::radians(camera->yaw)) * cos(glm::radians(camera->pitch));
	camera->front = glm::normalize(front);

	camera->right = glm::normalize(glm::cross(camera->front, camera->worldUp));
	camera->up =    glm::normalize(glm::cross(camera->right, camera->front));
}

void MoveCamera(Camera *camera, Camera_Movement direction, float deltaTime)
{
	float velocity = camera->moveSpeed * deltaTime;
	if (direction == FORWARD)
		camera->position += camera->front * velocity;
	if (direction == BACKWARD)
		camera->position -= camera->front * velocity;
	if (direction == LEFT)
		camera->position -= camera->right * velocity;
	if (direction == RIGHT)
		camera->position += camera->right * velocity;
	if (direction == UP)
		camera->position += camera->up * velocity;
	if (direction == DOWN)
		camera->position -= camera->up * velocity;
}

void CameraLook(Camera *camera, float xOffset, float yOffset, bool constrainPitch = true)
{
	camera->yaw   += camera->mouseSensitivity * xOffset;
	camera->pitch += camera->mouseSensitivity * -yOffset;

	//if (constrainPitch)
	{
		if (camera->pitch > 89.0f)
			camera->pitch = 89.0f;
		if (camera->pitch < -89.0f)
			camera->pitch = -89.0f;
	}
	UpdateCameraVectors(camera);
}