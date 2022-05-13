#include "RoboCatClientPCH.hpp"

/**
 * Vilandas Morrissey - D00218436
 */

AnimatedSpriteArtist::AnimatedSpriteArtist(const std::vector<AnimationData>& data) :
	mCurrentSpriteIndex(0),
	mFlipped(false),
	mLargestBoundsIndex(0),
	mSmallestBoundsIndex(0)
{
	for (auto& i : data)
	{
		mSpriteStates.emplace_back(AnimatedSprite(i, *TextureManager::sInstance->GetTexture(i.mTexture)));
	}

	CalculateLargestAndSmallest();
}

AnimatedSpriteArtist::~AnimatedSpriteArtist()
{
	Drawable::~Drawable();
	Transformable::~Transformable();
	RenderManager::sInstance->RemoveArtist(this);
}

void AnimatedSpriteArtist::UpdateCurrent(float inDeltaTime)
{
	CurrentSpriteState().UpdateCurrent(inDeltaTime);
}

AnimatedSprite AnimatedSpriteArtist::CurrentSpriteState() const
{
	return mSpriteStates[mCurrentSpriteIndex];
}

sf::FloatRect AnimatedSpriteArtist::GetBoundingRect() const
{
	return CurrentSpriteState().GetBoundingRect();
}

sf::FloatRect AnimatedSpriteArtist::GetLargestBounds() const
{
	AnimatedSprite sprite = mSpriteStates[mLargestBoundsIndex];
	return sprite.GetBoundingRect();
}

sf::FloatRect AnimatedSpriteArtist::GetSmallestBounds() const
{
	AnimatedSprite sprite = mSpriteStates[mSmallestBoundsIndex];
	return sprite.GetBoundingRect();
}

void AnimatedSpriteArtist::ChangeState(int index)
{
	if (index != mCurrentSpriteIndex)
	{
		assert(index >= 0 && index < mSpriteStates.size());

		CurrentSpriteState().Reset();
		mCurrentSpriteIndex = index;
	}
}

void AnimatedSpriteArtist::Pause()
{
	CurrentSpriteState().Pause();
}

void AnimatedSpriteArtist::Play()
{
	CurrentSpriteState().Play();
}

void AnimatedSpriteArtist::Flipped(bool flipped)
{
	mFlipped = flipped;
}

void AnimatedSpriteArtist::SetColor(sf::Color color)
{
	for (AnimatedSprite& state : mSpriteStates)
	{
		state.SetColor(color);
	}
}

void AnimatedSpriteArtist::SetColor(Vector3 color)
{
	SetColor(sf::Color(color.mX, color.mY, color.mZ));
}

AnimatedSprite& AnimatedSpriteArtist::CurrentSpriteState()
{
	return mSpriteStates[mCurrentSpriteIndex];
}

void AnimatedSpriteArtist::CalculateLargestAndSmallest()
{
	float largestProduct = std::numeric_limits<float>::min();
	float smallestProduct = std::numeric_limits<float>::max();

	for (int i = 0; i < mSpriteStates.size(); i++)
	{
		const sf::FloatRect bounds = mSpriteStates[i].GetBoundingRect();
		const float product = bounds.width * bounds.height;

		if (product < smallestProduct)
		{
			smallestProduct = product;
			mSmallestBoundsIndex = i;
		}

		if (product > largestProduct)
		{
			largestProduct = product;
			mLargestBoundsIndex = i;
		}
	}
}

void AnimatedSpriteArtist::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	CurrentSpriteState().DrawCurrent(target, states, mFlipped);
}
