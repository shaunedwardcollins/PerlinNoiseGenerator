#include "Button.h"
#include <iostream>

// ----- CONSTRUCTORS / DESTRUCTORS -----
Button::Button(TTF_TextEngine* textEngine, TTF_Font* font, SDL_FRect shape, char32_t text)
	: UIElement(textEngine, font, shape)
{
	buttonState = BUTTON_IDLE;
	glyph = text;
	this->text = "ERROR";
	isGlyph = true;
}
Button::Button(TTF_TextEngine* textEngine, TTF_Font* font, SDL_FRect shape, const char* text)
	: UIElement(textEngine, font, shape)
{
	buttonState = BUTTON_IDLE;
	this->text = text;
	glyph = NULL;
}

Button::~Button()
{

}

// ----- ACCESSORS -----
const bool Button::isPressed() const
{
	if (buttonState == BUTTON_PRESSED)
		return true;
	else
		return false;
}

void Button::handleEvents(const SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_EVENT_MOUSE_BUTTON_DOWN:
	{
		if (event.button.button == SDL_BUTTON_LEFT)
		{

			const float mx = event.button.x;
			const float my = event.button.y;

			if (mx >= m_srcRect.x && mx <= m_srcRect.x + m_srcRect.w
				&& my >= m_srcRect.y && my <= m_srcRect.y + m_srcRect.h)
			{
				buttonState = BUTTON_PRESSED;
			}
		}
		break;
	}
	case SDL_EVENT_MOUSE_BUTTON_UP:
	{
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			const float mx = event.button.x;
			const float my = event.button.y;

			if (mx >= m_srcRect.x && mx <= m_srcRect.x + m_srcRect.w
				&& my >= m_srcRect.y && my <= m_srcRect.y + m_srcRect.h)
				buttonState = BUTTON_HOVERED;
			else
				buttonState = BUTTON_IDLE;
		}
		break;
	}
	case SDL_EVENT_MOUSE_MOTION:
	{
		const float mx = event.motion.x;
		const float my = event.motion.y;

		if (mx >= m_srcRect.x && mx <= m_srcRect.x + m_srcRect.w
			&& my >= m_srcRect.y && my <= m_srcRect.y + m_srcRect.h)
			buttonState = BUTTON_HOVERED;
		else
			buttonState = BUTTON_IDLE;
		break;
	}
	}
}

void Button::update()
{
	if (isPressed())
		buttonState = BUTTON_HOVERED;
}

void Button::render(SDL_Renderer* renderer)
{
	// BACKGROUND COLOR BASED ON BUTTONSTATE AKA HOVERING
	switch (buttonState)
	{
	case BUTTON_IDLE:
		SDL_SetRenderDrawColor(renderer, m_background.r, m_background.g, m_background.b, m_background.a);
		break;
	case BUTTON_HOVERED:
		SDL_SetRenderDrawColor(renderer, m_hovered.r, m_hovered.g, m_hovered.b, m_hovered.a);
		break;
	case BUTTON_PRESSED:
		SDL_SetRenderDrawColor(renderer, m_outline.r, m_outline.g, m_outline.b, m_outline.a);
		break;
	}
	SDL_RenderFillRect(renderer, &m_srcRect);

	// OUTLINE COLOR
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderRect(renderer, &m_srcRect);
	
	if(fontSize)
		TTF_SetFontSize(m_font, fontSize);

	TTF_SetFontWrapAlignment(m_font, TTF_HORIZONTAL_ALIGN_CENTER);

	SDL_Surface* tmpSurface = nullptr;
	if (glyph)
		tmpSurface = TTF_RenderGlyph_Solid(m_font, glyph, SDL_Color{ 255, 255, 255, 255 });
	else
		tmpSurface = TTF_RenderText_Blended_Wrapped(m_font, text, strlen(text), SDL_Color{ 255, 255, 255, 255 }, 0);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);

	m_dstRect.w = (float)tmpSurface->w;
	m_dstRect.h = (float)tmpSurface->h;
	m_dstRect.x = m_srcRect.x + (m_srcRect.w - m_dstRect.w) / 2;
	m_dstRect.y = m_srcRect.y + (m_srcRect.h - m_dstRect.h) / 2;

	SDL_DestroySurface(tmpSurface);

	SDL_RenderTexture(renderer, texture, nullptr, &m_dstRect);
}