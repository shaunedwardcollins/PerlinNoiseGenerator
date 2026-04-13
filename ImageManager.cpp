#include "ImageManager.h"
#include "NoiseGenerator.h"

// ----- CONSTRUCTORS / DESTRUCTORS -----
ImageManager::ImageManager(Settings& settings, SDL_Renderer* renderer)
	: settings(settings), renderer(renderer)
{
	noiseRect = { (float)(1280.0f * 0.33f) - 256.0f, 140.0f, 
		(float)settings.imageSizes[settings.imageIndex], (float)settings.imageSizes[settings.imageIndex] };

	createTexture();

	noiseGenerator = new NoiseGenerator(texture);
}

ImageManager::~ImageManager()
{
	if (texture) SDL_DestroyTexture(texture);
	delete noiseGenerator;
}

// ----- FUNCTIONS -----
void ImageManager::handleEvents(const SDL_Event& event)
{
	std::lock_guard<std::mutex> lock(saveMutex);

	for (const auto& req : pendingSaves)
	{
		if (saveImage(req.renderer, req.texture, req.filename.c_str()))
			SDL_Log("Saved: %s", req.filename.c_str());
		else
			SDL_Log("Save failed for %s", req.filename.c_str());
	}

	pendingSaves.clear();
}

void ImageManager::update()
{
	noiseGenerator->update();
	noiseRect = { (float)((1280.0f * 0.33f) - settings.imageSizes[settings.imageIndex] / 2), 140.0f, (float)settings.imageSizes[settings.imageIndex], (float)settings.imageSizes[settings.imageIndex] };
}

void ImageManager::render(SDL_Renderer* renderer)
{
	if (noImage)
	{
		SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
		SDL_RenderFillRect(renderer, &noiseRect);
	}

	SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_LINEAR);
	SDL_RenderTexture(renderer, texture, nullptr, &noiseRect);
}

void ImageManager::generateImage()
{
	noiseGenerator->generate(settings.seed, 
		settings.octaves, settings.frequency, 
		settings.amplitude, settings.lacunarity, 
		settings.persistence, settings.contrast);
	if (noImage)
		noImage = false;
}

unsigned int ImageManager::generateRandomSeed()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());

	// 10 DIGIT RANGE TO MATCH MAX_DIGITS IN ENTRY BOX
	std::uniform_int_distribution<unsigned int> dist(1000000000U, 4294967295U);

	return dist(gen);
}

void ImageManager::showSaveTextureDialog(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Window* window)
{
	static const SDL_DialogFileFilter filters[] = {
	{ "PNG Image", "png" },
	{ "JPEG Image", "jpg;jpeg" },
	{ "BMP image", "bmp" },
	{ "All Files", "*" }
	};

	auto now = std::chrono::zoned_time{ std::chrono::current_zone(), std::chrono::system_clock::now() };

	std::ostringstream oss;
	oss << std::format("noise_{:%Y%m%d_%H%M}.png", now);
	std::string defaultName = oss.str();

	SaveTextureData* data = new SaveTextureData{
		this,
		renderer,
		texture
	};

	SDL_ShowSaveFileDialog(saveDialogCallback, data, window,
		filters, SDL_arraysize(filters), defaultName.c_str());
}

void ImageManager::setImageSize()
{
	noiseGenerator->setImageDim(settings.imageSizes[settings.imageIndex]);
}

unsigned int ImageManager::getSeed()
{
	return noiseGenerator->getSeed();
}

void ImageManager::createTexture()
{
	if (texture)
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}

	SDL_Surface* tmpSurface = SDL_CreateSurface(settings.imageSizes[settings.imageIndex],
		settings.imageSizes[settings.imageIndex], SDL_PIXELFORMAT_RGBA8888);
	texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);

	SDL_DestroySurface(tmpSurface);
}

bool ImageManager::saveImage(SDL_Renderer* renderer, SDL_Texture* texture, const char* filename)
{
	SDL_RenderTexture(renderer, texture, nullptr, &noiseRect);

	SDL_Rect tmpRect = { (int)noiseRect.x, (int)noiseRect.y, 
		settings.imageSizes[settings.imageIndex], settings.imageSizes[settings.imageIndex] };

	// READ PIXELS TO CREATE TEXTURE FROM SURFACE
	SDL_Surface* tmpSurface = SDL_RenderReadPixels(renderer, &tmpRect);
	if (!tmpSurface)
	{
		SDL_Log("SDL_RenderReadPixels failed: %s", SDL_GetError());
		return false;
	}

	// SAVES SURFACE AS PNG
	bool success = false;
	if (IMG_SavePNG(tmpSurface, filename))
		success = true;

	SDL_DestroySurface(tmpSurface);

	return success;
}

void SDLCALL ImageManager::saveDialogCallback(void* userdata, const char* const* filelist, int filter)
{
	SaveTextureData* data = static_cast<SaveTextureData*>(userdata);
	if (!data) return;

	if (!filelist || !filelist[0])
	{
		SDL_Log("Save dialog cancelled or error occurred.");
		delete data;
		return;
	}

	std::string chosenPath = filelist[0];

	{
		std::lock_guard<std::mutex> lock(data->manager->saveMutex);
		data->manager->pendingSaves.push_back({ data->renderer, data->texture, std::move(chosenPath) });
	}

	delete data;
}