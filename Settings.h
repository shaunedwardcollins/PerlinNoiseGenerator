#pragma once
#include <vector>

struct Settings
{
	std::vector<int> imageSizes =
	{ 64, 128, 256, 512 };
	int imageIndex = 3;

	int seed = 0;
	int octaves = 12;
	float frequency = 1.0f;
	float amplitude = 1.0f;
	float lacunarity = 2.0f;
	float persistence = 0.5f;
	float contrast = 1.2f;
};