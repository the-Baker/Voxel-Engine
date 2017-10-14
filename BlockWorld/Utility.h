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

int ThreeToOneD(glm::ivec3 vector, int width, int height)
{
	return (vector.x + width * (vector.y + vector.z * height));
}

int TwoToOneD(glm::ivec2 vector, int width)
{
	return (vector.x + width * vector.y);
}

