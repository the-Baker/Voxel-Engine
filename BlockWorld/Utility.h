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

