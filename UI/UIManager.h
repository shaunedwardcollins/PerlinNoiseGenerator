#pragma once
#include "Button.h"
#include "EntryBox.h"
#include "TextLabel.h"
#include "ValueLabel.h"
#include "..\Settings.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <map>

class UIManager
{
public:
	// ----- CONSTRUCTORS / DESTRUCTORS -----
	UIManager(Settings& settings, SDL_Window* window, SDL_Renderer* renderer);
	~UIManager();

	// ----- FUNCTIONS -----
	void handleEvents(const SDL_Event& event);
	void update();
	void render(SDL_Renderer* renderer);

	void setEntryBoxSeed();

	const bool generateRandomSeed() const;
	void resetRandomSeed() { randomSeed = false; }
	const bool generateImage() const;
	void resetGenerate() { generate = false; }
	const bool imageSizeChange() const;
	void resetImageSizeChange() { imageSize = false; }
	const bool saveImage() const;
	void resetSave() { save = false; }
	const bool settingsChange() const;
	void resetSettingChange() { settingChange = false; }
	

private:
	// ----- INITIALIZATION -----
	void initButtons();
	void initEntryBox();
	void initLabels();

	// ----- HELPERS -----
	std::string dtos(double d);

	// ----- VARIABLES -----
	Settings& settings;

	SDL_Window* window = nullptr;
	int windowWidth = 0;
	int windowHeight = 0;

	TTF_Font* font = nullptr;
	TTF_TextEngine* textEngine = nullptr;

	SDL_FRect settingsBounds = { 0, 0, 0, 0 };
	std::vector<SDL_FRect*> bounds;

	std::map<std::string, Button*> buttons;
	EntryBox* seedEntryBox = nullptr;
	std::map<std::string, UIElement*> labels;

	bool generate = false;
	bool imageSize = false;
	bool save = false;
	bool randomSeed = false;
	bool settingChange = false;
};

