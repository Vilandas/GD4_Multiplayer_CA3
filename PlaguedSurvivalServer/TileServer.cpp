#include "RoboCatServerPCH.hpp"

TileServer::TileServer() :
	mIsTop(),
	mActiveCollision(),
	mLeftTile(nullptr),
	mRightTile(nullptr),
	mBelowTiles()
{
}

void TileServer::HandleDying()
{
	if (!mBelowTiles.empty())
	{
		TileServer* tile = mBelowTiles.front();
		//World::sInstance->GetGameObjectsInLayer(tile->GetLayer());
		mBelowTiles.pop();
		tile->SetIsTop(mBelowTiles);
	}

	if (mLeftTile != nullptr)
	{
		mLeftTile->SetActiveCollision();
		mLeftTile->SetRightTile(nullptr);
	}

	if (mRightTile != nullptr)
	{
		mRightTile->SetActiveCollision();
		mRightTile->SetLeftTile(nullptr);
	}


	Tile::HandleDying();
	DangerTrigger::sInstance->RemoveDangerObject(this);
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}

void TileServer::SetLayer(Layers layer)
{
	if (GetLayer() == layer) return;

	//Tile::SetLayer(layer);
	//NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), TRS_State);
}

void TileServer::Trigger()
{
	if (mHealth == 0) return;

	if (mHealth > 0)
	{
		mHealth--;
		NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), TRS_Health);
	}

	if (mHealth == 0)
	{
		SetDoesWantToDie(true);
	}
}

void TileServer::AddBelowTile(TileServer* tile)
{
	mBelowTiles.push(tile);
}

void TileServer::SetIsTop()
{
	if (mIsTop) return;

	mIsTop = true;
	DangerTrigger::sInstance->AddDangerObject(this);
	SetActiveCollision();
}

void TileServer::SetIsTop(const std::queue<TileServer*>& belowTiles)
{
	SetIsTop();
	mBelowTiles = belowTiles;
}

void TileServer::SetActiveCollision()
{
	if (mActiveCollision) return;

	mActiveCollision = true;
	//SetLayer(Layers::kActivePlatforms);
}

void TileServer::SetLeftTile(TileServer* tile)
{
	mLeftTile = tile;
}

void TileServer::SetRightTile(TileServer* tile)
{
	mRightTile = tile;
}

uint32_t TileServer::Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const //35 bits total
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
