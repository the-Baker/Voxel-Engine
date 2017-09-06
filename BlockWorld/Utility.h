#pragma once


#define ARRAY_COUNT(ARRAY) sizeof(ARRAY) / sizeof(ARRAY[0])


int roundFloatToInt(float f)
{
	if (f >= 0)
		return int(f + 0.5f);
	else if (f <= 0)
		return int(f - 0.5f);
}

