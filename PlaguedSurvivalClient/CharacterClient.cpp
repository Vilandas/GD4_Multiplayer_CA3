#include "RoboCatClientPCH.hpp"

CharacterClient::CharacterClient() :
	mTimeLocationBecameOutOfSync(0.f),
	mTimeVelocityBecameOutOfSync(0.f),
	mView(sf::FloatRect(0, 0, 1920, 1080)),
	mCameraMoveConstraint()
{
	SetScale(0.5f);
	mArtist.reset(new AnimatedSpriteArtist(DataTables::CharacterData.ToVector()));
	mArtist->setScale(0.5f, 0.5f);
}

void CharacterClient::HandleDying()
{
	Character::HandleDying();
	RenderManager::sInstance->RemoveText(&mNameDisplay);

	//and if we're local, tell the hud so our health goes away!
	if (GetPlayerId() == NetworkManagerClient::sInstance->GetPlayerId())
	{
		RenderManager::sInstance->RemoveText(&mArrow);
		HUD::sInstance->SetPlayerHealth(0);
	}
}


void CharacterClient::Update()
{
	//is this the cat owned by us?
	if (GetPlayerId() == NetworkManagerClient::sInstance->GetPlayerId())
	{
		const Move* pendingMove = InputManager::sInstance->GetAndClearPendingMove();
		//in theory, only do this if we want to sample input this frame / if there's a new move ( since we have to keep in sync with server )
		if (pendingMove) //is it time to sample a new move...
		{
			float deltaTime = pendingMove->GetDeltaTime();

			//apply that input

			ProcessInput(deltaTime, pendingMove->GetInputState());

			//and simulate!

			SimulateMovement(deltaTime);

			//LOG( "Client Move Time: %3.4f deltaTime: %3.4f left rot at %3.4f", latestMove.GetTimestamp(), deltaTime, GetRotation() );
		}

		UpdateCamera();
		WindowManager::sInstance->setView(mView);
	}
	else
	{
		SimulateMovement(Timing::sInstance.GetDeltaTime());

		if (GetVelocity() == sf::Vector2f(0, 0))
		{
			//we're in sync if our velocity is 0
			mTimeLocationBecameOutOfSync = 0.f;
		}
	}

	mArtist->UpdateCurrent(Timing::sInstance.GetDeltaTime());
	mArtist->setPosition(GetLocationV2());
	mNameDisplay.setPosition(GetLocationV2() + sf::Vector2f(0, 90));
	mArrow.setPosition(GetLocationV2() + sf::Vector2f(15, -100));

	WindowManager::sInstance->draw(mNameDisplay);

	UpdateAnimationState();
}

void CharacterClient::UpdateAnimationState() const
{
	if (mVelocity.x != 0.f)
	{
		mArtist->ChangeState(static_cast<int>(CharacterAnimationState::kRun));

		if (mVelocity.x > 0)
		{
			mArtist->Flipped(false);
		}
		else if (mVelocity.x < 0)
		{
			mArtist->Flipped(true);
		}
	}
	else
	{
		mArtist->ChangeState(static_cast<int>(CharacterAnimationState::kIdle));
	}
}

void CharacterClient::UpdateCamera()
{
	if (mIsDead)
	{
		mView.setCenter(GetLocationV2());
		return;
	}

	const sf::Vector2f distance = GetLocationV2() - mView.getCenter();

	if (mCameraMoveConstraint || RoboMath::Length(distance) > 100)
	{
		mCameraMoveConstraint = true;

		const sf::Vector2f newPosition = distance * 0.02f;
		mView.setCenter(mView.getCenter() + newPosition);

		if (RoboMath::Length(newPosition) <= 0.2f)
		{
			mCameraMoveConstraint = false;
		}
	}
}

void CharacterClient::Read(InputMemoryBitStream& inInputStream)
{
	bool stateBit;

	uint32_t readState = 0;

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		uint32_t playerId;
		inInputStream.Read(playerId);

		if (GetPlayerId() == 0 && playerId != 0)
		{
			const auto& entry = *ScoreBoardManager::sInstance->GetEntry(playerId);
			mNameDisplay.setString(entry.GetPlayerName());
			mNameDisplay.setFont(*FontManager::sInstance->GetFont(Fonts::kCarlito));
			mNameDisplay.scale(0.75f, 0.75f);
			mNameDisplay.setFillColor(entry.GetSfColor());

			RoboMath::CentreOrigin(mNameDisplay);

			RenderManager::sInstance->AddText(&mNameDisplay);

			if (NetworkManagerClient::sInstance->GetPlayerId() == playerId)
			{
				mArrow.setString("V");
				mArrow.setFont(*FontManager::sInstance->GetFont(Fonts::kCarlito));
				mArrow.scale(1.f, 1.f);
				mArrow.setFillColor(entry.GetSfColor());
				RoboMath::CentreOrigin(mArrow);

				RenderManager::sInstance->AddText(&mArrow);
				RenderManager::sInstance->AddArtist(mArtist.get(), true);
			}
			else RenderManager::sInstance->AddArtist(mArtist.get());
		}

		SetPlayerId(playerId);
		readState |= CRS_PlayerId;
	}

	Vector3 oldLocation = GetLocation();
	sf::Vector2f oldVelocity = GetVelocity();

	Vector3 replicatedLocation;
	sf::Vector2f replicatedVelocity;

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		bool isDead = false;
		inInputStream.Read(isDead);
		replicatedVelocity.x = ReadWritePatterns::ReadFloat(inInputStream, 10);
		replicatedVelocity.y = ReadWritePatterns::ReadFloat(inInputStream, 11);
		SetVelocity(replicatedVelocity);

		replicatedLocation = ReadWritePatterns::ReadLocation(inInputStream);
		SetLocation(replicatedLocation);

		if (isDead && !mIsDead)
		{
			mIsDead = true;
			RenderManager::sInstance->RemoveArtist(mArtist.get());
		}

		readState |= CRS_Pose;
	}

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		inInputStream.Read(stateBit);
		mHorizontalDirection = stateBit ? 1.f : -1.f;
	}
	else
	{
		mHorizontalDirection = 0.f;
	}

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		Vector3 color;
		inInputStream.Read(color);
		SetColor(color);
		mArtist->SetColor(color);
		readState |= CRS_Color;
	}

	if (GetPlayerId() == NetworkManagerClient::sInstance->GetPlayerId())
	{
		DoClientSidePredictionAfterReplicationForLocalCharacter(readState);

		//if this is a create packet, don't interpolate
		if ((readState & CRS_PlayerId) == 0)
		{
			InterpolateClientSidePrediction(oldLocation, oldVelocity, false);
		}
	}
	else
	{
		DoClientSidePredictionAfterReplicationForRemoteCharacter(readState);

		//will this smooth us out too? it'll interpolate us just 10% of the way there...
		if ((readState & CRS_PlayerId) == 0)
		{
			InterpolateClientSidePrediction(oldLocation, oldVelocity, true);
		}

	}
}

void CharacterClient::DoClientSidePredictionAfterReplicationForLocalCharacter(uint32_t inReadState)
{
	if ((inReadState & CRS_Pose) != 0)
	{
		//simulate pose only if we received new pose- might have just gotten thrustDir
		//in which case we don't need to replay moves because we haven't warped backwards

		//all processed moves have been removed, so all that are left are unprocessed moves
		//so we must apply them...
		const MoveList& moveList = InputManager::sInstance->GetMoveList();

		for (const Move& move : moveList)
		{
			float deltaTime = move.GetDeltaTime();
			ProcessInput(deltaTime, move.GetInputState());

			SimulateMovement(deltaTime);
		}
	}
}


void CharacterClient::InterpolateClientSidePrediction(const Vector3& inOldLocation, const sf::Vector2f& inOldVelocity, bool inIsForRemoteCharacter)
{
	//if (inOldRotation != GetRotation() && !inIsForRemoteCharacter)
	//{
	//	LOG("ERROR! Move replay ended with incorrect rotation!", 0);
	//}

	float roundTripTime = NetworkManagerClient::sInstance->GetRoundTripTime();

	if (!RoboMath::Is2DVectorEqual(inOldLocation, GetLocation()))
	{
		//LOG( "ERROR! Move replay ended with incorrect location!", 0 );

		//have we been out of sync, or did we just become out of sync?
		float time = Timing::sInstance.GetFrameStartTime();
		if (mTimeLocationBecameOutOfSync == 0.f)
		{
			mTimeLocationBecameOutOfSync = time;
		}

		float durationOutOfSync = time - mTimeLocationBecameOutOfSync;
		if (durationOutOfSync < roundTripTime)
		{
			SetLocation(Lerp(inOldLocation, GetLocation(), inIsForRemoteCharacter ? (durationOutOfSync / roundTripTime) : 0.1f));
		}
	}
	else
	{
		//we're in sync
		mTimeLocationBecameOutOfSync = 0.f;
	}


	if (inOldVelocity != GetVelocity())
	{
		//LOG( "ERROR! Move replay ended with incorrect velocity!", 0 );

		//have we been out of sync, or did we just become out of sync?
		float time = Timing::sInstance.GetFrameStartTime();
		if (mTimeVelocityBecameOutOfSync == 0.f)
		{
			mTimeVelocityBecameOutOfSync = time;
		}

		//now interpolate to the correct value...
		float durationOutOfSync = time - mTimeVelocityBecameOutOfSync;
		if (durationOutOfSync < roundTripTime)
		{
			SetVelocity(RoboMath::Lerp(inOldVelocity, GetVelocity(), inIsForRemoteCharacter ? (durationOutOfSync / roundTripTime) : 0.1f));
		}
		//otherwise, fine...

	}
	else
	{
		//we're in sync
		mTimeVelocityBecameOutOfSync = 0.f;
	}

}


//so what do we want to do here? need to do some kind of interpolation...

void CharacterClient::DoClientSidePredictionAfterReplicationForRemoteCharacter(uint32_t inReadState)
{
	if ((inReadState & CRS_Pose) != 0)
	{

		//simulate movement for an additional RTT
		float rtt = NetworkManagerClient::sInstance->GetRoundTripTime();
		//LOG( "Other cat came in, simulating for an extra %f", rtt );

		//let's break into framerate sized chunks though so that we don't run through walls and do crazy things...
		float deltaTime = 1.f / 30.f;

		while (true)
		{
			if (rtt < deltaTime)
			{
				SimulateMovement(rtt);
				break;
			}
			else
			{
				SimulateMovement(deltaTime);
				rtt -= deltaTime;
			}
		}
	}
}

