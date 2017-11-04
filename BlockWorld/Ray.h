#pragma once
#include <glm\glm.hpp>
#include "Chunk.h"
#include "GameState.h"
#include "Utility.h"



glm::vec3 getRayDirection(Camera camera, glm::vec2 screenCoords, glm::vec2 screenDimensions)
{
	glm::mat4 viewMatrix = GetCameraViewMatrix(camera);
	glm::vec2 normalizedCoordinates = screenToNormalizedCoordinates(screenCoords, screenDimensions);
	glm::vec4 clipCoordinates = { normalizedCoordinates.x, normalizedCoordinates.y, -1.0f, 1.0f };
	glm::mat4 inverseProjectionMatrix = glm::inverse(glm::perspective(glm::radians(camera.fov), screenDimensions.x / screenDimensions.y, 0.1f, camera.viewDistance));
	glm::vec4 eyeCoordinates = inverseProjectionMatrix * clipCoordinates;
	eyeCoordinates = glm::vec4(eyeCoordinates.x, eyeCoordinates.y, -1.0f, 0.0f);
	glm::vec4 worldCoordinates = glm::inverse(viewMatrix) * eyeCoordinates;
	worldCoordinates = glm::normalize(worldCoordinates);
	return glm::vec3(worldCoordinates.x, worldCoordinates.y, worldCoordinates.z);
}

void initRay(Ray *ray, glm::vec3 origin, glm::vec3 direction)
{
	ray->origin = origin;
	ray->end = origin;
	ray->direction = glm::normalize(direction);
}

glm::vec3 findFirstBlock(float stepSize, float distance, Ray *ray, GameState *state)
{
	glm::vec3 incrementer;
	incrementer = ray->direction * stepSize;
	for (float i = 0.0f; i < distance; i += stepSize)
	{
		ray->end += incrementer;
		glm::vec2 chunkPos = posOfChunkAtWorldPos(ray->end);
		glm::vec3 blockPos = worldToChunkPosition(ray->end, chunkPos);

		long long int chunkKey = vec2ToInt64(chunkPos);
		int blockKey = vec3ToInt(blockPos);

		Chunk *chunk = &state->chunks.at(chunkKey);
		if (chunk->blocks.count(blockKey) == 1)
		{
			return ray->end;
		}
	}
	return glm::ivec3(0, 0, 0);
}

glm::vec3 findLastSpace(float stepSize, float distance, Ray *ray, GameState *state)
{
	glm::vec3 incrementer;
	incrementer = ray->direction * stepSize;
	for (float i = 0.0f; i < distance; i += stepSize)
	{
		ray->end += incrementer;
		glm::vec2 chunkPos = posOfChunkAtWorldPos(ray->end);
		glm::vec3 blockPos = worldToChunkPosition(ray->end, chunkPos);

		long long int chunkKey = vec2ToInt64(chunkPos);
		int blockKey = vec3ToInt(blockPos);

		Chunk *chunk = &state->chunks.at(chunkKey);
		if (chunk->blocks.count(blockKey) == 1)
		{
			return ray->end - (2.0f * incrementer);
		}
	}
	return glm::ivec3(0, 0, 0);
}