#pragma once

#include "camera.h"

enum Move_Direction {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};


struct Player
{
	float moveSpeed = 10.0f;
	float yaw = 0.0f;
	float pitch = 0.0f;
	float lookSensitivity = SENSITIVTY;
	Camera camera;
	glm::vec3 position;
	glm::vec3 cameraOffset = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 front;
	glm::vec3 right;
};


void initPlayer(Player *player)
{
	player->position = glm::vec3(5.0f, 7.0f, 5.0f);
	glm::vec3 front;
	front.x = cos(glm::radians(player->yaw)) * cos(glm::radians(player->pitch));
	front.y = sin(glm::radians(player->pitch));
	front.z = sin(glm::radians(player->yaw)) * cos(glm::radians(player->pitch));
	player->front = glm::normalize(front);
}

void updatePlayerVectors(Player *player)
{
	glm::vec3 front;
	front.x = cos(glm::radians(player->yaw)) * cos(glm::radians(player->pitch));
	front.y = sin(glm::radians(player->pitch));
	front.z = sin(glm::radians(player->yaw)) * cos(glm::radians(player->pitch));
	player->front = glm::normalize(front);

	player->right = glm::normalize(glm::cross(player->front, player->worldUp));
}

void updatePlayer(Player *player)
{
	player->camera.position = player->position;
	player->camera.yaw = player->yaw;
	player->camera.pitch = player->pitch;
	player->camera.front = player->front;
	player->camera.up = glm::vec3(0.0f, 1.0f, 0.0f);
	player->camera.mouseSensitivity = SENSITIVTY;
	player->camera.fov = ZOOM;
	player->camera.worldUp = player->worldUp;
	UpdateCameraVectors(&player->camera);
	updatePlayerVectors(player);
}

void movePlayer(Player *player, Move_Direction direction, float deltaTime)
{
	float velocity = player->moveSpeed * deltaTime;
	glm::vec3 front = player->front * glm::vec3(1.0f, 0.0f, 1.0f);
	glm::vec3 right = player->right * glm::vec3(1.0f, 0.0f, 1.0f);

	if (direction == FORWARD)
		player->position += front * velocity;
	if (direction == BACKWARD)
		player->position -= front * velocity;
	if (direction == LEFT)
		player->position -= right * velocity;
	if (direction == RIGHT)
		player->position += right * velocity;
	if (direction == UP)
		player->position += player->worldUp * velocity;
	if (direction == DOWN)
		player->position -= player->worldUp * velocity;
}




void playerLook(Player *player, float xOffset, float yOffset, bool constrainPitch = true)
{
	player->yaw += player->lookSensitivity * xOffset;
	player->pitch -= player->lookSensitivity * yOffset;

	if (constrainPitch)
	{
		if (player->pitch < -89.9f)
		{
			player->pitch = -89.9f;
		}
		else if (player->pitch > 89.9f)
		{
			player->pitch = 89.9f;
		}
	}
	updatePlayerVectors(player);
}