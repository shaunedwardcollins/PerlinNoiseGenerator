#pragma once
#include <vector>
#include <mutex>
#include <chrono>
#include <SDL3_image/SDL_image.h>
#include "NoiseGenerator.h"
#include "Settings.h"

struct SaveTextureData
{
	class ImageManager* manager;
	SDL_Renderer* renderer;
	SDL_Texture* texture;
};

struct SaveRequest
{
	SDL_Renderer* renderer;
	SDL_Texture* texture;
	std::string filename;
};

class ImageManager
{
public:
	// ----- CONSTRUCTORS / DESTRUCTORS -----
	ImageManager(Settings& settings, SDL_Renderer* renderer);
	~ImageManager();

	// ----- FUCNTIONS -----
	void handleEvents(const SDL_Event& event);
	void update();
	void render(SDL_Renderer* renderer);

	void generateImage();
	unsigned int generateRandomSeed();
	void showSaveTextureDialog(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Window* window);

	void setImageSize();
	unsigned int getSeed();
	SDL_Texture* getTexture() const { return texture; }

private:
	void createTexture();
	bool saveImage(SDL_Renderer* renderer, SDL_Texture* texture, const char* filename);
	static void SDLCALL saveDialogCallback(void* userdata, const char* const* filelist, int filter);

	// ----- VARIABLES -----
	Settings& settings;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* texture = nullptr;
	NoiseGenerator* noiseGenerator = nullptr;

	SDL_FRect noiseRect;

	bool noImage = true;

	std::vector<SaveRequest> pendingSaves;
	std::mutex saveMutex;
};