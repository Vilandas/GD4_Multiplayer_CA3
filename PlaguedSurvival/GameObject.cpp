#include "RoboCatPCH.hpp"

GameObject::GameObject() :
	mLayer(),
	mColor(Colors::White),
	mCollisionRadius(1.f),
	mBounds(0, 0, 1.f, 1.f),
	mRotation(0.f),
	mScale(1.0f),
	mIndexInWorld(-1),
	mDoesWantToDie(false),
	mNetworkId(0)
{
}

void GameObject::Update()
{
	//object don't do anything by default...	
}

Vector3 GameObject::GetForwardVector()	const
{
	//should we cache this when you turn?
	//SDL appears to use radians while SFML uses degrees - make sure to convert to radians
	//before using!
	return Vector3(sinf(RoboMath::ToRadians(mRotation)), -cosf(RoboMath::ToRadians(mRotation)), 0.f);
}

void GameObject::SetNetworkId(int inNetworkId)
{
	//this doesn't put you in the map or remove you from it
	mNetworkId = inNetworkId;

}

void GameObject::SetRotation(float inRotation)
{
	//should we normalize using fmodf?
	mRotation = inRotation;
}

void GameObject::SetLocation(const Vector3& inLocation)
{
	mLocation = inLocation;
	mBounds.left = mLocation.mX - (mBounds.width * 0.5f);
	mBounds.top = mLocation.mY - (mBounds.height * 0.5f);
}
