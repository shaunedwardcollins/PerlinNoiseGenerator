#pragma once
#include "UIElement.h"

class ValueLabel : public UIElement
{
public:
	// ----- CONSTRUCTORS / DESTRUCTORS -----
	ValueLabel(TTF_TextEngine* textEngine, TTF_Font* font, SDL_FRect shape, std::string value);
	~ValueLabel();

	// ----- FUNCTIONS -----
	void render(SDL_Renderer* renderer) override;
	void setValue(std::string value) override;

private:
	// ----- VARIABLES -----
	TTF_Text* value = nullptr;
};