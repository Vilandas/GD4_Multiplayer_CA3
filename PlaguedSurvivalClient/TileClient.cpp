#include "RoboCatClientPCH.hpp"

void TileClient::SetTexture(Textures texture)
{
	Tile::SetTexture(texture);
	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture(texture));
}

TileClient::TileClient()
{
	mSpriteComponent.reset(new SpriteComponent(this));
}
