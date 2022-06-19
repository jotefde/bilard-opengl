#pragma once
#include "utils.h"

#define WIDTH 1280
#define HEIGHT 720

float mapX(float x)
{
	if (x > WIDTH)
		x = WIDTH;
	else if (x < 0)
		x = 0;
	return (x / WIDTH) * 2 - 1;
}

float mapZ(float z)
{
	if (z > HEIGHT)
		z = HEIGHT;
	else if (z < 0)
		z = 0;
	return (z / HEIGHT) * 2 - 1;
}