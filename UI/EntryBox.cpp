#include "EntryBox.h"

// ----- CONSTRUCTORS / DESTRUCTORS -----
EntryBox::EntryBox(TTF_TextEngine* textEngine, TTF_Font* font, SDL_FRect shape, std::string text)
	: UIElement(textEngine, font, shape)
{
	this->text = text;
}

EntryBox::~EntryBox()
{
}

// ----- FUNCTIONS -----
void EntryBox::handleEvents(SDL_Window* window, const SDL_Event& event)
{
	if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
	{
		SDL_FPoint mousePos = { (float)event.button.x, (float)event.button.y };
		active = SDL_PointInRectFloat(&mousePos, &m_srcRect);
		if (active)
		{
			if (text == "Enter Seed")
				text = "";
			SDL_StartTextInput(window);
		}
		else
			SDL_StopTextInput(window);
	}

	if (this->active)
	{
		switch (event.type)
		{
		case SDL_EVENT_TEXT_INPUT:
		{
			const char* input = event.text.text;

			if (text.length() < MAX_DIGITS)
			{
				// Only allow numeric characters between 0 and 9
				for (const char* p = input; *p != '\0'; ++p)
				{
					if (*p >= '0' && *p <= '9')
						text += *p;
				}
			}

			if (!text.empty())
			{
				cursorPos = (int)text.length();
				textChanged = true;
			}
			break;
		}

		case SDL_EVENT_KEY_DOWN:
			switch (event.key.key)
			{
			case SDLK_BACKSPACE:
				if (!text.empty() && cursorPos > 0)
				{
					text.erase(cursorPos - 1, 1);
					cursorPos--;
				}
				break;
			case SDLK_DELETE:
				if (cursorPos < (int)text.length())
					text.erase(cursorPos, 1);
				break;
			case SDLK_LEFT:
				if (cursorPos > 0)
					cursorPos--;
				break;
			case SDLK_RIGHT:
				if (cursorPos < (int)text.length())
					cursorPos++;
				break;
			case SDLK_RETURN:

				// HANDLE PRESSING ENTER HERE IF NEED BE

				break;
			case SDLK_ESCAPE:
				active = false;
				SDL_StopTextInput(window);
				break;
			}
			break;
		}
	}
}

void EntryBox::update()
{
	if (textChanged && !text.empty())
		seed = stoi(text);
}
void EntryBox::render(SDL_Renderer* renderer)
{
	// BACKGROUND COLOR
	SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
	SDL_RenderFillRect(renderer, &m_srcRect);

	// OUTLINE COLOR
	SDL_SetRenderDrawColor(renderer, m_outline.r, m_outline.g, m_outline.b, m_outline.a);
	SDL_RenderRect(renderer, &m_srcRect);

	// DESTROY TEXTURE POINTER ON TEXT CHANGE
	SDL_Texture* texture = nullptr;
	if (textChanged && texture)
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}

	// CREATE TEXT TEXTURE TO ALIGN FONT / OR NOT!?
	TTF_SetFontWrapAlignment(m_font, TTF_HORIZONTAL_ALIGN_RIGHT);
	SDL_Surface* tmpSurface = TTF_RenderText_Blended_Wrapped(m_font, text.c_str(), text.length(), SDL_Color{255,255,255,255}, 0);
	if (tmpSurface)
	{
		texture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
		m_dstRect.x = m_srcRect.x + (m_srcRect.w - (float)tmpSurface->w) - 5.0f;  // 5.0f IS "PADDING"
		m_dstRect.w = (float)tmpSurface->w;
		m_dstRect.h = (float)tmpSurface->h;
		SDL_DestroySurface(tmpSurface);
		textChanged = false;
	}

	SDL_RenderTexture(renderer, texture, nullptr, &m_dstRect);
}

void EntryBox::setText(std::string text)
{
	this->text = text;
}