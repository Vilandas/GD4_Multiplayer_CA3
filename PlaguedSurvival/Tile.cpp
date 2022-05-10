#include "RoboCatPCH.hpp"

Tile::Tile()
	: mHealth(2)
	, mIsTop()
	, mActiveCollision()
	, mTexture()
	, mLeftTile(nullptr)
	, mRightTile(nullptr)
	, mBelowTiles()
{
	SetScale(0.5f);
	SetLayer(Layers::kPlatforms);
	SetBounds(sf::FloatRect(0, 0, 64, 64));
}

uint32_t Tile::Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const //35 bits total
{
	uint32_t writtenState = 0;

	if (inDirtyState & TRS_Pose) //26 bits
	{
		inOutputStream.Write(true);
		ReadWritePatterns::WriteLocationRounded(inOutputStream, GetLocation());

		writtenState |= TRS_Pose;
	}
	else
	{
		inOutputStream.Write(false);
	}

	if (inDirtyState & TRS_Health) // 3 bits
	{
		inOutputStream.Write(true);
		inOutputStream.Write(mHealth, 2);
		writtenState |= TRS_Health;
	}
	else
	{
		inOutputStream.Write(false);
	}

	if (inDirtyState & TRS_Texture) // 6 bits
	{
		inOutputStream.Write(true);
		inOutputStream.Write(static_cast<opt::Texture>(mTexture), opt::TextureBits);
		writtenState |= TRS_Texture;
	}
	else
	{
		inOutputStream.Write(false);
	}

	if (inDirtyState & TRS_State)
	{
		inOutputStream.Write(true);
		inOutputStream.Write(static_cast<opt::Layer>(GetLayer()), opt::LayerBits);
		writtenState |= TRS_State;
	}
	else
	{
		inOutputStream.Write(false);
	}

	return writtenState;
}

void Tile::Read(InputMemoryBitStream& inInputStream)
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
		if (GetLayer() == Layers::kPlatforms && layer == Layers::kActivePlatforms)
		{
			World::sInstance->SwapGameObjectLayer(*this, layer);
		}

		if (GetLayer() == Layers::kActivePlatforms)
		{
			WorldChunks::sInstance->AddToChunk(this, GetLayer());
		}
	}
}

void Tile::AddBelowTile(Tile* tile)
{
	mBelowTiles.push(tile);
}

void Tile::SetIsTop(bool is_new)
{
	if (mIsTop) return;

	mIsTop = true;
	//DangerTrigger::Instance().AddDangerObject(this);

	if (is_new)
	{
		mActiveCollision = true;
		SetLayer(Layers::kActivePlatforms);
		//World::sInstance->SwapGameObjectLayer(*this, Layers::kActivePlatforms);
		//WorldChunks::sInstance->AddToChunk(this, Layers::kActivePlatforms);
	}
	else
	{
		SetActiveCollision();
	}
}

void Tile::SetIsTop(const std::queue<Tile*>& below_tiles)
{
	SetIsTop();
	mBelowTiles = below_tiles;
}

void Tile::SetActiveCollision()
{
	if (mActiveCollision) return;

	mActiveCollision = true;
	SetLayer(Layers::kActivePlatforms);
	//World::sInstance->SwapGameObjectLayer(*this, Layers::kActivePlatforms);
	//WorldChunks::sInstance->AddToChunk(this, Layers::kActivePlatforms);
}

void Tile::SetLeftTile(Tile* tile)
{
	mLeftTile = tile;
}

void Tile::SetRightTile(Tile* tile)
{
	mRightTile = tile;
}