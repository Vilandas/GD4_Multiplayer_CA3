#include "RoboCatPCH.hpp"

Character::Character() :
	GameObject(),
	mPlayerId(0),
	mHorizontalDirection(),
	mIsJumping(),
	mAccelerationSpeed(1000),
	mDeceleration(4000),
	mGravity(200),
	mMaxVelocity(300, 800),
	mJumpForce(400),
	mCoyoteTime(0.25f),
	mAirTime(),
	mIsDead()
{
	SetLayer(Layers::kPlayers);
	SetBounds(sf::FloatRect(0, 0, 95, 128));
}

void Character::Accelerate(float inDeltaTime)
{
	mVelocity.x += mHorizontalDirection * mAccelerationSpeed * inDeltaTime;
}

void Character::Decelerate(float inDeltaTime)
{
	const float deceleration = mDeceleration * inDeltaTime;

	if (mHorizontalDirection == 0.f)
	{
		if (mVelocity.x > 0)
		{
			mVelocity.x = std::max(mVelocity.x - deceleration, 0.f);
		}
		else if (mVelocity.x < 0)
		{
			mVelocity.x = std::min(mVelocity.x + deceleration, 0.f);
		}
	}
}

void Character::ApplyGravity(float inDeltaTime)
{
	if (mVelocity.y > -300)
	{
		mVelocity.y += mGravity * inDeltaTime * 4;
	}
	else mVelocity.y += mGravity * inDeltaTime;
}

/// <summary>
/// Ensures the velocity does not exceed the entities max velocity
/// </summary>
void Character::ValidateVelocity()
{
	if (mVelocity.x > mMaxVelocity.x)
	{
		mVelocity.x = mMaxVelocity.x;
	}
	else if (mVelocity.x < -mMaxVelocity.x)
	{
		mVelocity.x = -mMaxVelocity.x;
	}

	if (mVelocity.y > mMaxVelocity.y)
	{
		mVelocity.y = mMaxVelocity.y;
	}
	else if (mVelocity.y < -mMaxVelocity.y)
	{
		mVelocity.y = -mMaxVelocity.y;
	}
}

void Character::ProcessInput(float inDeltaTime, const InputState& inInputState)
{
	mHorizontalDirection = inInputState.GetDesiredHorizontalDelta();
	if (inInputState.IsJumping() && !mIsDead)
	{
		Jump();
	}

}

void Character::SimulateMovement(float inDeltaTime)
{
	if (mIsDead)
	{
		Accelerate(inDeltaTime);
		Decelerate(inDeltaTime);
		ValidateVelocity();
		KeepInWorldBounds(inDeltaTime);
		mVelocity.y = 0;
		SetLocation(GetLocation() + mVelocity * inDeltaTime);
		return;
	}

	Accelerate(inDeltaTime);
	Decelerate(inDeltaTime);

	ApplyGravity(inDeltaTime);
	ValidateVelocity();

	ProcessCollisions(inDeltaTime);
	SetLocation(GetLocation() + mVelocity * inDeltaTime);
}

void Character::Update()
{

}

void Character::ProcessCollisions(float inDeltaTime)
{
	KeepInWorldBounds(inDeltaTime);

	std::set<GameObject*> collisions;

	PredictCollisionsWithChunks(inDeltaTime, Layers::kPlatforms, collisions);

	for (const GameObject* gameObject : collisions)
	{
		const CollisionLocation location = Collision::CollisionLocation(inDeltaTime, *this, *gameObject);
		BlockingCollision(location, gameObject);
	}
}

void Character::BlockingCollision(CollisionLocation location, const GameObject* collider)
{
	const sf::Vector2f velocity = GetVelocity();

	switch (location)
	{
	case CollisionLocation::kLeft:
		if (velocity.x < 0)
		{
			mVelocity.x = 0;
		}
		break;

	case CollisionLocation::kRight:
		if (velocity.x > 0)
		{
			mVelocity.x = 0;
		}
		break;

	case CollisionLocation::kTop:
		if (velocity.y < 0)
		{
			mVelocity.y = 0;
		}
		break;

	case CollisionLocation::kBottom:
		ResetJump();

		if (velocity.y > 0)
		{
			mVelocity.y = 0;
		}
		break;

	case CollisionLocation::kNone:
		break;
	}
}

void Character::KeepInWorldBounds(float inDeltaTime)
{
	sf::FloatRect bounds = GetBounds();
	bounds.left += mVelocity.x * inDeltaTime;
	bounds.top += mVelocity.y * inDeltaTime;

	if (bounds.left <= 0)
	{
		mVelocity.x = 0;
	}
	else if (bounds.left + bounds.width >= WorldInfo::WORLD_WIDTH)
	{
		mVelocity.x = 0;
	}

	if (bounds.top <= 0)
	{
		mVelocity.y = 0;
	}
	else if (bounds.top + bounds.height >= WorldInfo::WORLD_HEIGHT)
	{
		mVelocity.y = 0;
	}
}

void Character::Jump()
{
	if (!IsJumping() && mAirTime <= mCoyoteTime)
	{
		mIsJumping = true;
		SetVelocity(GetVelocity().x, -mJumpForce);
	}
}

void Character::ResetJump()
{
	mAirTime = 0.f;
	mIsJumping = false;
}

bool Character::IsGrounded() const
{
	return mAirTime > 0;
}

bool Character::IsJumping() const
{
	return mIsJumping;
}

uint32_t Character::Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const
{
	uint32_t writtenState = 0;

	if (inDirtyState & CRS_PlayerId)
	{
		inOutputStream.Write(true);
		inOutputStream.Write(GetPlayerId(), opt::PlayerIdBits);

		writtenState |= CRS_PlayerId;
	}
	else
	{
		inOutputStream.Write(false);
	}


	if (inDirtyState & CRS_Pose)
	{
		inOutputStream.Write(true);

		inOutputStream.Write(mIsDead);
		ReadWritePatterns::WriteFloat(inOutputStream, mVelocity.x, 10);
		ReadWritePatterns::WriteFloat(inOutputStream, mVelocity.y, 11);
		ReadWritePatterns::WriteLocation(inOutputStream, GetLocation());

		writtenState |= CRS_Pose;
	}
	else
	{
		inOutputStream.Write(false);
	}

	//always write mThrustDir- it's just two bits
	if (mHorizontalDirection != 0.f)
	{
		inOutputStream.Write(true);
		inOutputStream.Write(mHorizontalDirection > 0.f);
	}
	else
	{
		inOutputStream.Write(false);
	}


	if (inDirtyState & CRS_Color)
	{
		inOutputStream.Write(true);
		ReadWritePatterns::WriteColorRounded(inOutputStream, GetColor());

		writtenState |= CRS_Color;
	}
	else
	{
		inOutputStream.Write(false);
	}

	return writtenState;
}
