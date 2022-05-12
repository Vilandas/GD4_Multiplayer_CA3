#include "RoboCatClientPCH.hpp"

//void TileClient::Trigger()
//{
//	Tile::Trigger();
//
//	if (mHealth == 1)
//	{
//		mCrackSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture(Textures::kCrack));
//	}
//}

void TileClient::SetTexture(Textures texture)
{
	Tile::SetTexture(texture);
	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture(texture));
}

TileClient::TileClient()
{
	mSpriteComponent.reset(new SpriteComponent(this));
	mCrackSpriteComponent.reset(new SpriteComponent(this));
}

void TileClient::Read(InputMemoryBitStream& inInputStream)
{
	bool stateBit;

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		const Vector3 location = ReadWritePatterns::ReadLocationRounded(inInputStream);
		SetLocation(location);
	}

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		inInputStream.Read(mHealth, 2);

		if (mHealth == 1)
		{
			mCrackSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture(Textures::kCrack));
		}
	}


	inInputStream.Read(stateBit);
	if (stateBit)
	{
		opt::Texture texture;
		inInputStream.Read(texture, opt::TextureBits);
		SetTexture(static_cast<Textures>(texture));
	}

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		opt::Layer inLayer;
		inInputStream.Read(inLayer, opt::LayerBits);

		const auto layer = static_cast<Layers>(inLayer);
		SetLayer(layer);
	}
}