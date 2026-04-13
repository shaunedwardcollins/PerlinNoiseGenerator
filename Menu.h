#pragma once
#include "UI/UIManager.h"
#include "ImageManager.h"
#include "Settings.h"

class Menu
{
public:
	// ----- CONSTRUCTORS / DESTRUCTORS -----
	Menu();
	virtual ~Menu();

	// ----- FUNCTIONS -----
	void runLoop();
	void handleEvents();
	void update();
	void render();

private:
	// ----- INITIALIZATION -----
	void initWindow();
	void initUIManager();
	void initImageManager();

	// ----- VARIABLES -----
	SDL_Window* window = nullptr;
	std::string title = "Perlin Noise Generator";
	int windowWidth = 1280;
	int windowHeight = 720;
	bool isRunning = false;
	SDL_Renderer* renderer = nullptr;

	std::unique_ptr<Settings> settings;
	UIManager* uiManager = nullptr;
	ImageManager* imageManager = nullptr;
};