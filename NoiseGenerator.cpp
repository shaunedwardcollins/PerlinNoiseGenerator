#include "NoiseGenerator.h"
#include <iostream>

// ----- CONSTRUCTORS / DESTRUCTORS -----
NoiseGenerator::NoiseGenerator(SDL_Texture* texture)
{
    this->texture = texture;
    setPixels();

    gradients.resize(TABLE_SIZE);
    permutation.resize(TABLE_SIZE * 2);
}

NoiseGenerator::~NoiseGenerator()
{ 
    if (this->texture) SDL_DestroyTexture(texture);

    delete[] pixels;
    pixels = nullptr;
}

// ----- FUNCTIONS -----
void NoiseGenerator::update()
{
    if (generated)
    {
        texture->w = imageDim;
        texture->h = imageDim;
        SDL_UpdateTexture(texture, nullptr, pixels, imageDim * 4);
        generated = false;
    }
}

void NoiseGenerator::generate(unsigned int seed, int octaves, float frequency, 
    float amplitude, float lacunarity, float persistence, float contrast)
{
    if (seed != 0 || currentSeed == 0)
    {
        setSeed(seed != 0 ? seed : 12345);
    }

    for (int px = 0; px < imageDim; ++px)
    {
        for (int py = 0; py < imageDim; ++py)
        {
            float val = 0.0f;
            float freq = frequency;
            float amp = amplitude;
            int octs = octaves;

            for (int i = 0; i < octs; ++i)
            {
                val += perlin(px * freq / 64.0f, py * freq / 64.0f) * amp;
                freq *= lacunarity;
                amp *= persistence;
            }

            val *= contrast;

            if (val > 1.0f) val = 1.0f;
            else if (val < -1.0f) val = -1.0f;

            int color = (int)(((val + 1.0f) * 0.5) * 255.0f);

            int index = (py * imageDim + px) * 4;

            // Set pixel color
            pixels[index] = color;     // R
            pixels[index + 1] = color; // G
            pixels[index + 2] = color; // B
            pixels[index + 3] = 255;   // A
        }
    }
    generated = true;
    std::cout << "Generation Complete" << std::endl;
}

void NoiseGenerator::setSeed(unsigned int seed)
{
    currentSeed = seed;

    std::mt19937 rng(seed);
    std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * 3.14159265f);

    // FILL GRADIENTS WITH RANDOM UNIT VECTORS
    // AND CREATE PERMUTATION TABLE
    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        float angle = angleDist(rng);
        gradients[i].x = std::sin(angle);
        gradients[i].y = std::cos(angle);
        permutation[i] = i;
    }

    // SHUFFLE PERMUTATION TABLE WITH SEED 
    std::shuffle(permutation.begin(), permutation.begin() + TABLE_SIZE, rng);

    // DUPLICATE PERMUATION TABLE
    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        permutation[TABLE_SIZE + 1] = permutation[i];
    }
}

void NoiseGenerator::setImageDim(int imageDim)
{
    this->imageDim = imageDim;

    delete[] pixels;
    pixels = nullptr;

    setPixels();
}

void NoiseGenerator::setPixels()
{
    pixels = new Uint8[imageDim * imageDim * 4];
}

float NoiseGenerator::perlin(float x, float y) const
{
    int x0 = (int)x;
    int y0 = (int)y;
    int x1 = (x0 + 1);
    int y1 = (y0 + 1);

    float sx = x - (float)x0;
    float sy = y - (float)y0;

    float u = fade(sx);
    float v = fade(sy);

    // interpolate top corners
    float n00 = dotGridGradient(x0, y0, x, y);
    float n10 = dotGridGradient(x1, y0, x, y);

    // interpolate bottom corners
    float n01 = dotGridGradient(x0, y1, x, y);
    float n11 = dotGridGradient(x1, y1, x, y);

    float ix0 = lerp(n00, n10, u);
    float ix1 = lerp(n01, n11, u);

    return lerp(ix0, ix1, v);
}

float NoiseGenerator::fade(float t) const
{
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float NoiseGenerator::lerp(float a, float b, float t) const
{
    return a + t * (b - a);
}

float NoiseGenerator::dotGridGradient(int ix, int iy, float x, float y) const
{
    Vector2 gradient = getGradient(ix, iy);

    float dx = x - (float)ix;
    float dy = y - (float)iy;

    return dx * gradient.x + dy * gradient.y;
}

Vector2 NoiseGenerator::getGradient(int x, int y) const
{
    int hash = permutation[permutation[x & TABLE_MASK] + (y & TABLE_MASK)];
    return gradients[hash];
}