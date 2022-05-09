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
	mCameraMoveConstraint(),
	mIsCameraTarget()
{
	SetLayer(Layers::kPlayers);
	//SetCollisionRadius(60.f);
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
	mVelocity.y += mGravity * inDeltaTime;
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
	mIsJumping = inInputState.IsJumping();

}

void Character::SimulateMovement(float inDeltaTime)
{
	//simulate us...
	//AdjustVelocityByThrust(inDeltaTime);

	Accelerate(inDeltaTime);
	Decelerate(inDeltaTime);
	ApplyGravity(inDeltaTime);

	ValidateVelocity();
	ProcessCollisions();
	
	SetLocation(GetLocation() + mVelocity * inDeltaTime);
}

void Character::Update()
{

}

void Character::ProcessCollisions()
{
	////right now just bounce off the sides..
	//ProcessCollisionsWithScreenWalls();

	//float sourceRadius = GetCollisionRadius();
	//Vector3 sourceLocation = GetLocation();

	////now let's iterate through the world and see what we hit...
	////note: since there's a small number of objects in our game, this is fine.
	////but in a real game, brute-force checking collisions against every other object is not efficient.
	////it would be preferable to use a quad tree or some other structure to minimize the
	////number of collisions that need to be tested.
	//for (auto goIt = World::sInstance->GetGameObjects().begin(), end = World::sInstance->GetGameObjects().end(); goIt != end; ++goIt)
	//{
	//	GameObject* target = goIt->get();
	//	if (target != this && !target->DoesWantToDie())
	//	{
	//		//simple collision test for spheres- are the radii summed less than the distance?
	//		Vector3 targetLocation = target->GetLocation();
	//		float targetRadius = target->GetCollisionRadius();

	//		Vector3 delta = targetLocation - sourceLocation;
	//		float distSq = delta.LengthSq2D();
	//		float collisionDist = (sourceRadius + targetRadius);
	//		if (distSq < (collisionDist * collisionDist))
	//		{
	//			//first, tell the other guy there was a collision with a cat, so it can do something...

	//			if (target->HandleCollisionWithCharacter(this))
	//			{
	//				//okay, you hit something!
	//				//so, project your location far enough that you're not colliding
	//				Vector3 dirToTarget = delta;
	//				dirToTarget.Normalize2D();
	//				Vector3 acceptableDeltaFromSourceToTarget = dirToTarget * collisionDist;
	//				//important note- we only move this cat. the other cat can take care of moving itself
	//				SetLocation(targetLocation - acceptableDeltaFromSourceToTarget);


	//				Vector3 relVel = mVelocity;

	//				//if other object is a cat, it might have velocity, so there might be relative velocity...
	//				Character* targetChar = target->GetAsCharacter();
	//				if (targetChar)
	//				{
	//					relVel -= targetChar->mVelocity;
	//				}

	//				//got vel with dir between objects to figure out if they're moving towards each other
	//				//and if so, the magnitude of the impulse ( since they're both just balls )
	//				float relVelDotDir = Dot2D(relVel, dirToTarget);

	//				if (relVelDotDir > 0.f)
	//				{
	//					Vector3 impulse = relVelDotDir * dirToTarget;

	//					if (targetChar)
	//					{
	//						mVelocity -= impulse;
	//						mVelocity *= mCatRestitution;
	//					}
	//					else
	//					{
	//						mVelocity -= impulse * 2.f;
	//						mVelocity *= mWallRestitution;
	//					}

	//				}
	//			}
	//		}
	//	}
	//}

}

void Character::ProcessCollisionsWithScreenWalls()
{
	//Vector3 location = GetLocation();
	//float x = location.mX;
	//float y = location.mY;

	//float vx = mVelocity.x;
	//float vy = mVelocity.y;

	//float radius = GetCollisionRadius();

	////if the cat collides against a wall, the quick solution is to push it off
	//if ((y + radius) >= WorldInfo::WORLD_HEIGHT && vy > 0)
	//{
	//	mVelocity.mY = -vy * mWallRestitution;
	//	location.mY = WorldInfo::WORLD_HEIGHT - radius;
	//	SetLocation(location);
	//}
	//else if (y - radius <= 0 && vy < 0)
	//{
	//	mVelocity.mY = -vy * mWallRestitution;
	//	location.mY = radius;
	//	SetLocation(location);
	//}

	//if ((x + radius) >= WorldInfo::WORLD_WIDTH && vx > 0)
	//{
	//	mVelocity.mX = -vx * mWallRestitution;
	//	location.mX = WorldInfo::WORLD_WIDTH - radius;
	//	SetLocation(location);
	//}
	//else if (x - radius <= 0 && vx < 0)
	//{
	//	mVelocity.mX = -vx * mWallRestitution;
	//	location.mX = radius;
	//	SetLocation(location);
	//}
}

uint32_t Character::Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const
{
	uint32_t writtenState = 0;

	if (inDirtyState & CRS_PlayerId)
	{
		inOutputStream.Write(true);
		inOutputStream.Write(GetPlayerId());

		writtenState |= CRS_PlayerId;
	}
	else
	{
		inOutputStream.Write(false);
	}


	if (inDirtyState & CRS_Pose)
	{
		inOutputStream.Write(true);

		sf::Vector2f velocity = mVelocity;
		inOutputStream.Write(velocity.x);
		inOutputStream.Write(velocity.y);

		Vector3 location = GetLocation();
		inOutputStream.Write(location.mX);
		inOutputStream.Write(location.mY);

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
		inOutputStream.Write(GetColor());

		writtenState |= CRS_Color;
	}
	else
	{
		inOutputStream.Write(false);
	}

	return writtenState;
}