#include "RoboCatPCH.hpp"

Tile::Tile()
	: mHealth(2)
	, mTexture()
{
	SetScale(0.5f);
	GameObject::SetLayer(Layers::kPlatforms);
	SetBounds(sf::FloatRect(0, 0, 64, 64));
}

void Tile::HandleDying()
{
	WorldChunks::sInstance->RemoveFromChunk(this, Layers::kActivePlatforms);
}

void Tile::SetLayer(const Layers layer)
{
	if (layer == GetLayer()) return;

	World::sInstance->SwapGameObjectLayer(this, Layers::kPlatforms, Layers::kActivePlatforms);
	GameObject::SetLayer(layer);
	WorldChunks::sInstance->AddToChunk(this, layer);
}
