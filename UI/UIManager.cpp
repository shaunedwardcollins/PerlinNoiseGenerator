#include "UIManager.h"

// ----- CONSTRUCTORS / DESTRUCTORS -----
UIManager::UIManager(Settings& settings, SDL_Window* window, SDL_Renderer* renderer)
	: settings(settings), window(window)
{
	SDL_GetWindowSize(this->window, &windowWidth, &windowHeight);

	TTF_Init();

	const char* font_path = "Assets/unifont-16.0.04.ttf";
	font = TTF_OpenFont(font_path, 30.0f);
	if (!font)
		std::cout << "Failed to load font" << SDL_GetError() << std::endl;
	
	textEngine = TTF_CreateRendererTextEngine(renderer);

	settingsBounds = { (float)(windowWidth * 0.66f) + 5, 5.0f, (float)(windowWidth * 0.33f) - 10.0f, (float)windowHeight - 10.0f };

	initButtons();
	initEntryBox();
	initLabels();

	randomSeed = false;
}

UIManager::~UIManager()
{
	for (auto& l : labels)
		delete l.second;
	labels.clear();

	if (seedEntryBox) delete seedEntryBox;
	seedEntryBox = nullptr;

	for (auto& b : buttons)
		delete b.second;
	buttons.clear();

	for (auto& b : bounds)
		delete b;
	bounds.clear();

	if (font) TTF_CloseFont(font);
	if (textEngine) TTF_DestroyRendererTextEngine(textEngine);

	TTF_Quit();

	if (window) window = nullptr;
}

// ----- INITIALIZATION -----
void UIManager::initButtons()
{
	float upBtnX = (float)(windowWidth * 0.66f) + 40.0f;
	float downBtnX = (float)(windowWidth * 0.66f) + 165.0f;
	float btnW = 35.0f;
	float btnH = 35.0f;
	
	char32_t random = U'\u27F3';
	char32_t up = U'\u25B2';
	char32_t down = U'\u25BC';

	float genX = (float)(windowWidth * 0.33f) - 100;
	float savY = (float)(windowHeight - 55.0f);

	buttons["Generate"] = new Button(textEngine, font, { genX, 15.0f, 200.0f, 50.0f }, "Generate");

	buttons["ImageSizeUp"] = new Button(textEngine, font, { genX - 10.0f, 105.0f, 30.0f, 30.0f }, up);
	buttons["ImageSizeDown"] = new Button(textEngine, font, { genX + 180.0f, 105.0f, 30.0f, 30.0f }, down);

	buttons["Save Image"] = new Button(textEngine, font, { genX, savY, 200.0f, 40.0f }, "Save Image");

	buttons["Random Seed"] = new Button(textEngine, font, { upBtnX, 110.0f, btnW, btnH }, random);

	buttons["Enter Seed"] = new Button(textEngine, font, { 1150.f, 110.0f, 100.0f, btnH }, "Enter");

	buttons["OctUp"] = new Button(textEngine, font, { upBtnX, 200.0f, btnW, btnH }, up);
	buttons["OctDown"] = new Button(textEngine, font, { downBtnX, 200.0f, btnW, btnH }, down);

	buttons["FreqUp"] = new Button(textEngine, font, { upBtnX, 290.0f, btnW, btnH }, up);
	buttons["FreqDown"] = new Button(textEngine, font, { downBtnX, 290.0f, btnW, btnH }, down);

	buttons["AmpUp"] = new Button(textEngine, font, { upBtnX, 380.0f, btnW, btnH }, up);
	buttons["AmpDown"] = new Button(textEngine, font, { downBtnX, 380.0f, btnW, btnH }, down);

	buttons["LacUp"] = new Button(textEngine, font, { upBtnX, 470.0f, btnW, btnH }, up);
	buttons["LacDown"] = new Button(textEngine, font, { downBtnX, 470.0f, btnW, btnH }, down);

	buttons["PersUp"] = new Button(textEngine, font, { upBtnX, 560.0f, btnW, btnH }, up);
	buttons["PersDown"] = new Button(textEngine, font, { downBtnX, 560.0f, btnW, btnH }, down);

	buttons["ConUp"] = new Button(textEngine, font, { upBtnX, 650.0f, btnW, btnH }, up);
	buttons["ConDown"] = new Button(textEngine, font, { downBtnX, 650.0f, btnW, btnH }, down);
}

void UIManager::initEntryBox()
{
	seedEntryBox = new EntryBox(textEngine, font, { 925.0f, 110.0f, 220.0f, 35.0f }, "Enter Seed");
}

void UIManager::initLabels()
{
	float textLabelX = (float)(windowWidth * 0.66f) + 20;
	float valueLabelX = (float)(windowWidth * 0.66f) + 80;

	float textLabelW = 200.0f;
	float valueLabelW = 80.0f;

	float textLabelH = 40.0f;
	float valueLabelH = 35.0f;

	float imageSizeX = (float)(windowWidth * 0.33f) - 75;

	labels["ImageLabel"] = new ValueLabel(textEngine, font, { imageSizeX, 75.0f, 150.0f, 25.0f }, "Image Size:");
	labels["ImageLabel"]->setFontSize(24.0f);

	labels["ImageSize"] = new ValueLabel(textEngine, font, { imageSizeX, 105.0f, 150.0f, 30.0f }, std::to_string(settings.imageSizes[settings.imageIndex]) + " x " + std::to_string(settings.imageSizes[settings.imageIndex]));
	labels["ImageSize"]->setFontSize(28.0f);

	labels["Settings"] = new ValueLabel(textEngine, font, { (float)(windowWidth * 0.66f) + 10.0f, 10.0f, (float)(windowWidth * 0.33f) - 20.0f, 50.0f }, "Settings");
	labels["Settings"]->setFontSize(48.0f);

	labels["Seed"] = new TextLabel(textEngine, font, { textLabelX, 65.0f, textLabelW, textLabelH }, "Seed");

	labels["Octaves"] = new TextLabel(textEngine, font, { textLabelX, 155.0f, textLabelW, textLabelH }, "Octaves");
	labels["OctValue"] = new ValueLabel(textEngine, font, { valueLabelX, 200.0f, valueLabelW, valueLabelH }, std::to_string(settings.octaves));

	labels["Frequency"] = new TextLabel(textEngine, font, { textLabelX, 245.0f, textLabelW, textLabelH }, "Frequency");
	labels["FreqValue"] = new ValueLabel(textEngine, font, { valueLabelX, 290.0f, valueLabelW, valueLabelH }, dtos(settings.frequency).c_str());

	labels["Amplitude"] = new TextLabel(textEngine, font, { textLabelX, 335.0f, textLabelW, textLabelH }, "Amplitude");
	labels["AmpValue"] = new ValueLabel(textEngine, font, { valueLabelX, 380.0f, valueLabelW, valueLabelH }, dtos(settings.amplitude).c_str());

	labels["Lacunarity"] = new TextLabel(textEngine, font, { textLabelX, 425.0f, textLabelW, textLabelH }, "Lacunarity");
	labels["LacValue"] = new ValueLabel(textEngine, font, { valueLabelX, 470.0f, valueLabelW, valueLabelH }, dtos(settings.lacunarity).c_str());

	labels["Persistence"] = new TextLabel(textEngine, font, { textLabelX, 515.0f, textLabelW, textLabelH }, "Persistence");
	labels["PersValue"] = new ValueLabel(textEngine, font, { valueLabelX, 560.0f, valueLabelW, valueLabelH }, dtos(settings.persistence).c_str());

	labels["Contrast"] = new TextLabel(textEngine, font, { textLabelX, 605.0f, textLabelW, textLabelH }, "Contrast");
	labels["ConValue"] = new ValueLabel(textEngine, font, { valueLabelX, 650.0f, valueLabelW, valueLabelH }, dtos(settings.contrast).c_str());
}

// ----- FUNCTIONS -----
void UIManager::handleEvents(const SDL_Event& event)
{
	for (auto& b : buttons)
		b.second->handleEvents(event);

	seedEntryBox->handleEvents(window, event);
}

void UIManager::update()
{
	seedEntryBox->update();

	if (seedEntryBox->textChange())
	{
		settings.seed = seedEntryBox->getSeed();
	}

	// BUTTONS
	if (buttons["Generate"]->isPressed())
		generate = true;

	if (buttons["ImageSizeUp"]->isPressed())
	{
		settings.imageIndex++;
		if (settings.imageIndex > settings.imageSizes.size() - 1)
			settings.imageIndex = 0;
		labels["ImageSize"]->setValue(std::to_string(settings.imageSizes[settings.imageIndex]) + " x " + std::to_string(settings.imageSizes[settings.imageIndex]));
		imageSize = true;
	}

	if (buttons["ImageSizeDown"]->isPressed())
	{
		settings.imageIndex--;
		if (settings.imageIndex < 0)
			settings.imageIndex = settings.imageSizes.size() - 1;
		labels["ImageSize"]->setValue(std::to_string(settings.imageSizes[settings.imageIndex]) + " x " + std::to_string(settings.imageSizes[settings.imageIndex]));
		imageSize = true;
	}

	if (buttons["Save Image"]->isPressed())
		save = true;

	if (buttons["Random Seed"]->isPressed())
		randomSeed = true;

	if (buttons["Enter Seed"]->isPressed())
		generate = true;

	if (buttons["OctUp"]->isPressed())
	{
		settings.octaves += (int)1;
		labels["OctValue"]->setValue(std::to_string(settings.octaves));
		settingChange = true;
	}

	if (buttons["OctDown"]->isPressed())
	{
		settings.octaves -= (int)1;
		labels["OctValue"]->setValue(std::to_string(settings.octaves));
		settingChange = true;
	}

	if (buttons["FreqUp"]->isPressed())
	{
		settings.frequency += (float)0.1;
		labels["FreqValue"]->setValue(dtos(settings.frequency).c_str());
		settingChange = true;
	}

	if (buttons["FreqDown"]->isPressed())
	{
		settings.frequency -= (float)0.1;
		labels["FreqValue"]->setValue(dtos(settings.frequency).c_str());
		settingChange = true;
	}

	if (buttons["AmpUp"]->isPressed())
	{
		settings.amplitude += (float)0.1;
		labels["AmpValue"]->setValue(dtos(settings.amplitude).c_str());
		settingChange = true;
	}

	if (buttons["AmpDown"]->isPressed())
	{
		settings.amplitude -= (float)0.1;
		labels["AmpValue"]->setValue(dtos(settings.amplitude).c_str());
		settingChange = true;
	}

	if (buttons["LacUp"]->isPressed())
	{
		settings.lacunarity += (float)0.1;
		labels["LacValue"]->setValue(dtos(settings.lacunarity).c_str());
		settingChange = true;
	}

	if (buttons["LacDown"]->isPressed())
	{
		settings.lacunarity -= (float)0.1;
		labels["LacValue"]->setValue(dtos(settings.lacunarity).c_str());
		settingChange = true;
	}

	if (buttons["PersUp"]->isPressed())
	{
		settings.persistence += (float)0.1;
		labels["PersValue"]->setValue(dtos(settings.persistence).c_str());
		settingChange = true;
	}

	if (buttons["PersDown"]->isPressed())
	{
		settings.persistence -= (float)0.1;
		labels["PersValue"]->setValue(dtos(settings.persistence).c_str());
		settingChange = true;
	}

	if (buttons["ConUp"]->isPressed())
	{
		settings.contrast += (float)0.1;
		labels["ConValue"]->setValue(dtos(settings.contrast).c_str());
		settingChange = true;
	}

	if (buttons["ConDown"]->isPressed())
	{
		settings.contrast -= (float)0.1;
		labels["ConValue"]->setValue(dtos(settings.contrast).c_str());
		settingChange = true;
	}

	// SHOULD BE AT BOTTOM OF BUTTON LIST
	for (auto& b : buttons)
		b.second->update();
}

void UIManager::render(SDL_Renderer* renderer)
{
	// BACKGROUND FOR SETTINGS MENU
	SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
	SDL_RenderFillRect(renderer, &settingsBounds);

	// BACKGROUND FOR INDIVIDUAL SETTINGS
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 125);
	for (int i = 0; i < 7; i++)
	{
		bounds.push_back(new SDL_FRect{ (float)(windowWidth * 0.66f) + 35.0f, (i * 90) + 85.0f , 375.0f, 65.0f });
		SDL_RenderFillRect(renderer, bounds[i]);
	}

	// BUTTONS
	for (auto& b : buttons)
		b.second->render(renderer);

	// ENTRY BOX
	seedEntryBox->render(renderer);

	// LABELS
	for (auto& l : labels)
		l.second->render(renderer);
}

void UIManager::setEntryBoxSeed()
{
	seedEntryBox->setText(std::to_string(settings.seed));
}

const bool UIManager::generateImage() const
{
	if (generate)
		return true;
	else
		return false;
}

const bool UIManager::imageSizeChange() const
{
	if (imageSize)
		return true;
	else
		return false;
}

const bool UIManager::saveImage() const
{
	if (save)
		return true;
	else
		return false;
}

const bool UIManager::generateRandomSeed() const
{
	if (randomSeed)
		return true;
	else
		return false;
}

const bool UIManager::settingsChange() const
{
	if (settingChange)
		return true;
	else
		return false;
}

// ----- HELPERS -----
std::string UIManager::dtos(double d)
{
	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << d;
	return ss.str();
}