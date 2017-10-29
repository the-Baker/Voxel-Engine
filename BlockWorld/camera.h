#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>



const float YAW = -90.0f;
const float PITCH = 0.0f;

const float SENSITIVTY = 0.2f;
const float ZOOM = 65.0f;

struct Camera
{
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;


	float mouseSensitivity;
	float fov;
	float viewDistance = 450.0f;
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
	//camera->up =    glm::normalize(glm::cross(camera->right, camera->front));
}



void CameraLook(Camera *camera, float xOffset, float yOffset, bool constrainPitch = true)
{
	camera->yaw   += camera->mouseSensitivity * xOffset;
	camera->pitch += camera->mouseSensitivity * -yOffset;

	if (constrainPitch)
	{
		if (camera->pitch > 89.0f)
			camera->pitch = 89.0f;
		if (camera->pitch < -89.0f)
			camera->pitch = -89.0f;
	}
	UpdateCameraVectors(camera);
}