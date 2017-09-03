#pragma once


#define ARRAY_COUNT(ARRAY) sizeof(ARRAY) / sizeof(ARRAY[0])


int roundFloatToInt(float f)
{
	return int(f + 0.5f);
}
