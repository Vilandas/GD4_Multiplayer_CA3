#include "RoboCatServerPCH.hpp"

namespace
{
	const float kRespawnDelay = 3.f;
}

ClientProxy::ClientProxy(const SocketAddress& inSocketAddress, const string& inName, uint32_t inGamesWon, opt::PlayerId inPlayerId) :
	mDeliveryNotificationManager(false, true),
	mSocketAddress(inSocketAddress),
	mName(inName),
	mPlayerId(inPlayerId),
	mGamesWon(inGamesWon),
	mTimeToRespawn(0.f),
	mIsLastMoveTimestampDirty(false)
{
	UpdateLastPacketTime();
}


void ClientProxy::UpdateLastPacketTime()
{
	mLastPacketFromClientTime = Timing::sInstance.GetTimef();
}

void ClientProxy::HandleCatDied()
{
	mTimeToRespawn = Timing::sInstance.GetFrameStartTime() + kRespawnDelay;
}

void ClientProxy::RespawnCatIfNecessary()
{
	if (mTimeToRespawn != 0.f && Timing::sInstance.GetFrameStartTime() > mTimeToRespawn)
	{
		static_cast<Server*> (Engine::sInstance.get())->SpawnCharacterForPlayer(mPlayerId);
		mTimeToRespawn = 0.f;
	}
}


