#include "RoboCatClientPCH.hpp"

std::unique_ptr< TextureManager >		TextureManager::sInstance;

void TextureManager::StaticInit()
{
	sInstance.reset(new TextureManager());
}

TextureManager::TextureManager()
{
	CacheTexture(Textures::kCat, "../Assets/doc.png");
	CacheTexture(Textures::kCrack, "../Assets/Textures/Tiles/crack.png");
	CacheTexturePattern(Textures::kDirt1, Textures::kDirt9, "../Assets/Textures/Tiles/Dirt/dirt");
}

bool TextureManager::CacheTexture(Textures inTextureName, const char* inFileName)
{
	TexturePtr newTexture(new sf::Texture());
	if (!newTexture->loadFromFile(inFileName))
	{
		return false;
	}

	mNameToTextureMap[inTextureName] = newTexture;

	return true;
}

void TextureManager::CacheTexturePattern(Textures startTexture, Textures lastTexture, const std::string& locationPrefix)
{
	int i = 1;
	for (int texture = static_cast<int>(startTexture); texture <= static_cast<int>(lastTexture); texture++)
	{
		std::string path = locationPrefix + std::to_string(i) + ".png";
		CacheTexture(static_cast<Textures>(texture), path.c_str());
		i++;
	}
}

TexturePtr TextureManager::GetTexture(const Textures inTexture)
{
	return mNameToTextureMap[inTexture];
}