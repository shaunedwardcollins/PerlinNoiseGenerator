#include "TextLabel.h"

// ----- CONSTRUCTORS / DESTRUCTORS -----
TextLabel::TextLabel(TTF_TextEngine* textEngine, TTF_Font* font, SDL_FRect shape, const char* text)
	: UIElement(textEngine, font, shape)
{
	this->text = text;
	m_dstRect = { m_srcRect.x, m_srcRect.y + 5.0f, 0.0f, 0.0f };
}

TextLabel::~TextLabel()
{
	delete[] text;
}

// ----- FUNCTIONS -----
void TextLabel::render(SDL_Renderer* renderer)
{
	// BACKGROUND COLOR
	SDL_SetRenderDrawColor(renderer, m_background.r, m_background.g, m_background.b, m_background.a);
	SDL_RenderFillRect(renderer, &m_srcRect);

	// OUTLINE COLOR
	SDL_SetRenderDrawColor(renderer, m_outline.r, m_outline.g, m_outline.b, m_outline.a);
	SDL_RenderRect(renderer, &m_srcRect);

	if (fontSize)
		TTF_SetFontSize(m_font, fontSize);

	// CREATE TEXT TEXTURE TO ALIGN TEXT
	TTF_SetFontWrapAlignment(m_font, TTF_HORIZONTAL_ALIGN_CENTER);
	SDL_Surface* tmpSurface = TTF_RenderText_Blended_Wrapped(m_font, text, strlen(text), SDL_Color{255,255,255,255}, 0);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	
	// USE TMPSURFACE W, H TO PREVENT TEXT STRETCHING
	m_dstRect.w = (float)tmpSurface->w;
	m_dstRect.h = (float)tmpSurface->h;
	m_dstRect.x = m_srcRect.x + 10.0f;

	if (tmpSurface) SDL_DestroySurface(tmpSurface);

	SDL_RenderTexture(renderer, texture, nullptr, &m_dstRect);
}
