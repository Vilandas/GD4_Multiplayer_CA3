#include "RoboCatClientPCH.hpp"

TileClient::TileClient()
{
	mSpriteComponent.reset(new SpriteComponent(this));
	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture(Textures::kDirt1));
}
