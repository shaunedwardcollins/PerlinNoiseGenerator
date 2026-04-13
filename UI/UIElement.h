#pragma once
#include <string>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class UIElement
{
public:
	// ----- CONSTRUCTORS / DESTRUCTORS -----
	explicit UIElement(TTF_TextEngine* textEngine, TTF_Font* font, SDL_FRect shape)
		: m_textEngine(textEngine), m_font(font), m_srcRect(shape)
	{ }

	~UIElement()
	{
		m_font = nullptr;
		m_textEngine = nullptr;
	}

	// ----- FUNCTIONS -----
	virtual void render(SDL_Renderer* renderer) = 0;
	virtual void setValue(std::string value) { }
	void setFontSize(float fontSize) { this->fontSize = fontSize; }

protected:
	// ----- Variables -----
	TTF_TextEngine* m_textEngine = nullptr;

	TTF_Font* m_font = nullptr;
	float fontSize = 30.0f;

	SDL_FRect m_srcRect = { 0.0f, 0.0f, 0.0f, 0.0f };
	SDL_FRect m_dstRect = { m_srcRect.x, m_srcRect.y, 0, 0 };

	SDL_Color m_background = { 50, 0, 65, 255 };
	SDL_Color m_outline = { 255, 255, 255, 255 };
	SDL_Color m_hovered = { 125, 125, 125, 255 };
};