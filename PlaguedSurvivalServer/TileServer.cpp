#include "RoboCatServerPCH.hpp"

TileServer::TileServer()
{
}

void TileServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}

void TileServer::Trigger()
{
	Tile::Trigger();

	if (mHealth <= 0)
	{
		DangerTrigger::sInstance->RemoveDangerObject(this);
		SetDoesWantToDie(true);
	}
}

void TileServer::SetIsTop(bool isNew)
{
	Tile::SetIsTop(isNew);

	if (!isNew)
	{
		DangerTrigger::sInstance->AddDangerObject(this);
	}
}


bool TileServer::HandleCollisionWithCharacter(Character* inCharacter)
{
	//SetDoesWantToDie(true);

	//ScoreBoardManager::sInstance->IncScore(inCat->GetPlayerId(), 1);

	return false;
}
