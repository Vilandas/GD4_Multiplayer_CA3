#include "RoboCatServerPCH.hpp"

TileServer::TileServer()
{
}

void TileServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}


bool TileServer::HandleCollisionWithCat(RoboCat* inCat)
{
	//SetDoesWantToDie(true);

	//ScoreBoardManager::sInstance->IncScore(inCat->GetPlayerId(), 1);

	return false;
}