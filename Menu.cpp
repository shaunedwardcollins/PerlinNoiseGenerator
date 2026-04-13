#include "Menu.h"
#include <iostream>

// ----- CONSTRUCTORS/DESTRUCTORS -----
Menu::Menu() 
	: settings(std::make_unique<Settings>())
{
	initWindow();
	initUIManager();
	initImageManager();
}

Menu::~Menu()
{
	if (imageManager) delete imageManager;
	imageManager = nullptr;

	if (uiManager) delete uiManager;
	uiManager = nullptr;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
}

// ----- INITIALIZATION -----
void Menu::initWindow()
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		std::cout << "SDL_Init failed" << SDL_GetError() << std::endl;
	}

	int flags = 0;
	window = SDL_CreateWindow(title.c_str(), windowWidth, windowHeight, flags);
	if (!window)
	{
		std::cout << "SDL_CreateWindow failed" << SDL_GetError() << std::endl;
		SDL_Quit();
		isRunning = false;
	}
	
	renderer = SDL_CreateRenderer(window, NULL);
	if (renderer)
		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
	else
	{
		std::cout << "SDL_CreateRenderer failed" << SDL_GetError() << std::endl;
		SDL_DestroyWindow(window);
		SDL_Quit();
		isRunning = false;
	}

	isRunning = true;
}

void Menu::initUIManager()
{
	uiManager = new UIManager(*settings, window, renderer);
}

void Menu::initImageManager()
{
	imageManager = new ImageManager(*settings, renderer);
}

// ----- FUNCTIONS -----
void Menu::runLoop()
{
	while (isRunning)
	{
		handleEvents();
		update();
		render();
	}
}

void Menu::handleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		imageManager->handleEvents(event);

		if (event.type == SDL_EVENT_QUIT)
		{
			isRunning = false;
			continue;
		}

		if (event.type == SDL_EVENT_MOUSE_MOTION)
		{
			const float x = event.motion.x;
			const float y = event.motion.y;
			const float xrel = event.motion.xrel;
			const float yrel = event.motion.yrel;
			Uint32 state = event.motion.state;
		}

		uiManager->handleEvents(event);
	}
}

void Menu::update()
{
	if (uiManager->saveImage())
	{
		imageManager->showSaveTextureDialog(renderer, imageManager->getTexture(), window);
		uiManager->resetSave();
	}

	if (uiManager->generateRandomSeed())
	{
		settings->seed = imageManager->generateRandomSeed();
		imageManager->generateImage();
		uiManager->setEntryBoxSeed();
		uiManager->resetRandomSeed();
	}

	if (uiManager->generateImage())
	{
		imageManager->generateImage();
		uiManager->resetGenerate();
		settings->seed = imageManager->getSeed();
	}

	if (uiManager->imageSizeChange())
	{
		imageManager->setImageSize();
		uiManager->resetImageSizeChange();
	}

	if (uiManager->settingsChange())
	{
		imageManager->generateImage();
		uiManager->resetSettingChange();
	}

	imageManager->update();

	uiManager->update();
}

void Menu::render()
{
	// BACKGROUND COLOR
	SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
	SDL_RenderClear(renderer);

	uiManager->render(renderer);

	imageManager->render(renderer);

	SDL_RenderPresent(renderer);
}