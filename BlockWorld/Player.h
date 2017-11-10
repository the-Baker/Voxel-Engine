#pragma once

#include "camera.h"
#include "Physics.h"

#define JUMPVALUE 2000.0f

enum Move_Direction 
{
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
	float pickDistance = 16.0f;
	Camera camera;
	glm::vec3 position;
	glm::vec3 hitBoxDimensions = glm::vec3(0.5f, 1.61803398875f, 0.5f);
	glm::vec3 cameraOffset = glm::vec3(0.0f, 1.61803398875f, 0.0f);
	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 velocity = { 0.0f, 0.0f, 0.0f };
	glm::vec3 acceleration = { 0.0f, 0.0f, 0.0f };

	bool canFly = false;
	bool onGround = false;
};


void initPlayer(Player *player)
{
	player->position = glm::vec3(8.0f, 43.0f, 8.0f);
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
	player->camera.position = player->position + player->cameraOffset;
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