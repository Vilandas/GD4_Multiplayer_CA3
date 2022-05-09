#include "RoboCatServerPCH.hpp"

TileServer::TileServer()
{
}

void TileServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}


bool TileServer::HandleCollisionWithCharacter(Character* inCharacter)
{
	//SetDoesWantToDie(true);

	//ScoreBoardManager::sInstance->IncScore(inCat->GetPlayerId(), 1);

	return false;
}