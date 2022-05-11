#include "RoboCatClientPCH.hpp"

void TileClient::Trigger()
{
	Tile::Trigger();

	if (mHealth == 1)
	{
		mCrackSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture(Textures::kCrack));
	}
}

void TileClient::SetTexture(Textures texture)
{
	Tile::SetTexture(texture);
	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture(texture));
}

TileClient::TileClient()
{
	mSpriteComponent.reset(new SpriteComponent(this));
}
