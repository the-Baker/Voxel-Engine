#pragma once

#define ARRAY_COUNT(ARRAY) sizeof(ARRAY) / sizeof(ARRAY[0])

#include "GameState.h"

DebugState debugState = {};


int roundFloatToInt(float f)
{
	if (f >= 0)
		return int(f + 0.5f);
	else if (f <= 0)
		return int(f - 0.5f);
	else
		return 0;
}

glm::ivec3 roundToMultiple(glm::ivec3 toRound, int multiple)
{
	glm::ivec3 result;
	result.x = (toRound.x / multiple) * multiple;
	result.y = (toRound.y / multiple) * multiple;
	result.z = (toRound.z / multiple) * multiple;
	return toRound;
}

glm::ivec2 roundToMultiple(glm::ivec2 toRound, int multiple)
{
	glm::ivec3 result;
	result.x = (toRound.x / multiple) * multiple;
	result.y = (toRound.y / multiple) * multiple;
	return toRound;
}

int ThreeToOneD(glm::ivec3 vector, int width, int height)
{
	return (vector.x + width * (vector.y + vector.z * height));
}

int TwoToOneD(glm::ivec2 vector, int width)
{
	return (vector.x + width * vector.y);
}

int vec3ToInt(glm::ivec3 vector)
{
	int result = 0;
	result = (vector.x << 24);
	result = result | (vector.y << 16);
	result = result | (vector.z << 8);
	return result;
}

glm::ivec3 intToVec3(int integer)
{
	glm::ivec3 result;
	result.x = integer >> 24;
	result.y = (integer >> 16) & (0x000000ff);
	result.z = (integer >> 8)  & (0x000000ff);
	return result;
}

long long int vec2ToInt64(glm::ivec2 vector)
{
	long long int result = 0;
	result = (long long int)vector.x << 32;
	result = result | (long long int)vector.y;
	return result;
}

glm::ivec2 int64ToVec2(long long int integer)
{
	glm::ivec2 result;
	result.x = integer >> 32;
	result.y = integer & 0x00000000ffffffff;
	return result;
}

glm::vec2 screenToNormalizedCoordinates(glm::vec2 screenCoords, glm::vec2 screenDimensions)
{
	glm::vec2 result;
	result.x = (2.0f * screenCoords.x) / screenDimensions.x - 1.0f;
	result.y = (2.0f * screenCoords.y) / screenDimensions.y - 1.0f;

	return result;
}