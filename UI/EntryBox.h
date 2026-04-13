#pragma once
#include "UIElement.h"

class EntryBox : public UIElement
{
public:
	// ----- CONSTRUCTORS / DESTRUCTORS -----
	EntryBox(TTF_TextEngine* textEngine, TTF_Font* font, SDL_FRect shape, std::string text);
	~EntryBox();

	// ----- FUNCTIONS -----
	void handleEvents(SDL_Window* window, const SDL_Event& event);
	void update();
	void render(SDL_Renderer* renderer) override;

	int getSeed() const { return seed; }
	void setText(std::string text);
	bool textChange() const { return textChanged; }

private:
	// ----- VARIABLES -----
	std::string text = "";
	int cursorPos = 0;
	
	bool active = false;
	bool textChanged = false;

	int seed = 0;
	const size_t MAX_DIGITS = 10;
};

