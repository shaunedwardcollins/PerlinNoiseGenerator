#include "ValueLabel.h"

// ----- CONSTRUCTORS / DESTRUCTORS -----

ValueLabel::ValueLabel(TTF_TextEngine* textEngine, TTF_Font* font, SDL_FRect shape, std::string value)
	: UIElement(textEngine, font, shape)
{
	this->value = TTF_CreateText(textEngine, font, value.c_str(), value.length());
	m_dstRect = { m_srcRect.x, m_srcRect.y, 0, 0 };
}

ValueLabel::~ValueLabel()
{
	if (value) TTF_DestroyText(value);
}

// ----- FUNCTIONS -----
void ValueLabel::render(SDL_Renderer* renderer)
{
	// BACKGROUND COLOR
	SDL_SetRenderDrawColor(renderer, m_background.r, m_background.g, m_background.b, m_background.a);
	SDL_RenderFillRect(renderer, &m_srcRect);

	// OUTLINE COLOR
	SDL_SetRenderDrawColor(renderer, m_outline.r, m_outline.g, m_outline.b, m_outline.a);
	SDL_RenderRect(renderer, &m_srcRect);

	if(fontSize)
		TTF_SetFontSize(m_font, fontSize);

	// GET TEXT SIZE TO SET POSITION
	int tw = 0, th = 0;
	TTF_GetTextSize(value, &tw, &th);
	float tx = m_srcRect.x + (m_srcRect.w - tw) / 2.0f;
	float ty = m_srcRect.y + (m_srcRect.h - th) / 2.0f;

	TTF_SetTextColorFloat(value, 255, 255, 255, 255);

	TTF_DrawRendererText(value, tx, ty);
}

void ValueLabel::setValue(std::string value)
{
	if (this->value) TTF_DestroyText(this->value);
	this->value = TTF_CreateText(m_textEngine, m_font, value.c_str(), value.length());
}