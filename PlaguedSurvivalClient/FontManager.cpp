#include "RoboCatClientPCH.hpp"

std::unique_ptr<FontManager> FontManager::sInstance;

void FontManager::StaticInit()
{
	sInstance.reset(new FontManager());
}

FontManager::FontManager()
{
	CacheFont(Fonts::kCarlito, "../Assets/Fonts/Carlito-Regular.ttf");
}

FontPtr FontManager::GetFont(const Fonts font)
{
	return mNameToFontMap[font];
}

bool FontManager::CacheFont(const Fonts font, const char* inFileName)
{
	FontPtr newFont(new sf::Font());
	if (!newFont->loadFromFile(inFileName))
		return false;

	mNameToFontMap[font] = newFont;
	return true;
}

