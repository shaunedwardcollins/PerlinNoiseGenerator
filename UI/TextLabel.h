#pragma once
#include "UIElement.h"

class TextLabel : public UIElement
{
public:
	// ----- CONSTRUCTORS / DESTRUCTORS -----
	TextLabel(TTF_TextEngine* textEngine, TTF_Font* font, SDL_FRect shape, const char* text);
	~TextLabel();

	// ----- FUNCTIONS -----
	void render(SDL_Renderer* renderer) override;

private:
	// ----- VARIABLES -----
	const char* text;
};