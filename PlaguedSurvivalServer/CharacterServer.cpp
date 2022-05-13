#include "RoboCatServerPCH.hpp"

CharacterServer::CharacterServer() :
	mTimeOfNextShot(0.f),
	mTimeBetweenShots(0.2f)
{}

void CharacterServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}

void CharacterServer::Update()
{
	Character::Update();

	Vector3 oldLocation = GetLocation();
	sf::Vector2f oldVelocity = GetVelocity();

	ClientProxyPtr client = NetworkManagerServer::sInstance->GetClientProxy(GetPlayerId());
	if (client)
	{
		MoveList& moveList = client->GetUnprocessedMoveList();
		for (const Move& unprocessedMove : moveList)
		{
			const InputState& currentState = unprocessedMove.GetInputState();

			float deltaTime = unprocessedMove.GetDeltaTime();

			ProcessInput(deltaTime, currentState);
			SimulateMovement(deltaTime);

			//LOG( "Server Move Time: %3.4f deltaTime: %3.4f left rot at %3.4f", unprocessedMove.GetTimestamp(), deltaTime, GetRotation() );

		}

		moveList.Clear();
	}

	sf::FloatRect bounds = GetBounds();

	if (bounds.top + bounds.height > WorldInfo::WORLD_HEIGHT - 100)
	{
		mIsDead = true;
		SetVelocity(0, 0);
	}

	if (!RoboMath::Is2DVectorEqual(oldLocation, GetLocation()) ||
		oldVelocity != GetVelocity())
	{
		NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), CRS_Pose);
	}
}

void CharacterServer::TakeDamage(int inDamagingPlayerId)
{
	SetDoesWantToDie(true);

	ClientProxyPtr clientProxy = NetworkManagerServer::sInstance->GetClientProxy(GetPlayerId());
	if (clientProxy)
	{
		clientProxy->HandleCatDied();
	}
}

