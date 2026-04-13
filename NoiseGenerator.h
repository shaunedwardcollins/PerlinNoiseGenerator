#pragma once
#include "Vector2.h"
#include <string>
#include <cmath>
#include <random>
#include <algorithm>
#include <SDL3/SDL.h>

class NoiseGenerator
{
public:
	// ----- CONSTRUCTORS / DESTRUCTORS -----
	NoiseGenerator(SDL_Texture* texture);
	~NoiseGenerator();

	// ----- FUNCTIONS -----
	void update();

	void generate(unsigned int seed, int octaves, float frequency, float amplitude,
		float lacunarity, float persistence, float contrast);

	void setSeed(unsigned int seed);
	unsigned int getSeed() const { return currentSeed; }
	void setImageDim(int imageDim);
	
private:
	// ----- FUNCTIONS -----
	void setPixels();
	float perlin(float x, float y) const;
	float fade(float t) const;
	float dotGridGradient(int ix, int iy, float x, float y) const;
	Vector2 getGradient(int x, int y) const;
	float lerp(float a, float b, float t) const;

	// ----- VARIABLES -----
	SDL_Texture* texture = nullptr;
	int imageDim = 512;
	Uint8* pixels;

	static const int TABLE_SIZE = 256;
	static const int TABLE_MASK = TABLE_SIZE - 1;

	std::vector<Vector2> gradients;
	std::vector<int> permutation;

	unsigned int currentSeed = 0;

	bool generated = false;
};