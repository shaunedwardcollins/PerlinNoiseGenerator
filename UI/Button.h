#pragma once
#include "UIElement.h"

enum ButtonState
{
	BUTTON_IDLE = 0,
	BUTTON_HOVERED,
	BUTTON_PRESSED
};

class Button : public UIElement
{
public:
	// ----- CONSTRUCTORS / DESTRUCTORS -----
	Button(TTF_TextEngine* textEngine, TTF_Font* font, SDL_FRect shape, char32_t text);
	Button(TTF_TextEngine* textEngine, TTF_Font* font, SDL_FRect shape, const char* text);
	~Button();

	// ----- ACCESSORS -----
	const bool isPressed() const;

	// ----- FUNCTIONS -----
	void handleEvents(const SDL_Event& event);
	void update();
	void render(SDL_Renderer* renderer) override;

private:
	// ----- VARIABLES -----
	short unsigned buttonState;
	const char* text;
	char32_t glyph;
	bool isGlyph = false;
};