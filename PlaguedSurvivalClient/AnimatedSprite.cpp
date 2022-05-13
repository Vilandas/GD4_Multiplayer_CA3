#include "RoboCatClientPCH.hpp"

/**
 * Vilandas Morrissey - D00218436
 */

AnimatedSprite::AnimatedSprite(const AnimationData& data, const sf::Texture& texture)
	: mTimeSinceLastFrame(0)
	, mTimePerFrame(data.mTimePerFrame)
	, mCurrentIndex(0)
	, mLoop(true)
	, mPaused(false)
{
	for (int i = 0; i < data.mFrames; i++)
	{
		sf::IntRect rect(i * data.mFrameWidth, 0, data.mFrameWidth, data.mFrameHeight);
		sf::Sprite sprite(texture, rect);

		RoboMath::CentreOrigin(sprite);
		mSprites.emplace_back(sprite);
	}
}

void AnimatedSprite::DrawCurrent(sf::RenderTarget& target, sf::RenderStates& states, bool flipped) const
{
	sf::Sprite sprite = CurrentSprite();

	if (flipped)
	{
		sf::Vector2f scale = sprite.getScale();
		sprite.setScale(-scale.x, scale.y);
	}

	target.draw(sprite, states);
}

void AnimatedSprite::UpdateCurrent(float inDeltaTime)
{
	if (!mPaused)
	{
		mTimeSinceLastFrame += 0.1f;
		mTimeSinceLastFrame += inDeltaTime;

		if (mTimeSinceLastFrame > mTimePerFrame)
		{
			Advance();
			mTimeSinceLastFrame -= mTimePerFrame;
		}
	}
}

sf::Sprite AnimatedSprite::CurrentSprite() const
{
	return mSprites[mCurrentIndex];
}

sf::FloatRect AnimatedSprite::GetBoundingRect()
{
	return CurrentSprite().getGlobalBounds();
}

void AnimatedSprite::Pause()
{
	mPaused = true;
}

void AnimatedSprite::Play()
{
	mPaused = false;
}

void AnimatedSprite::Reset()
{
	mCurrentIndex = 0;
	mTimeSinceLastFrame = 0;
}

void AnimatedSprite::SetColor(sf::Color color)
{
	for (sf::Sprite& sprite : mSprites)
	{
		sprite.setColor(color);
	}
}

void AnimatedSprite::Advance()
{
	mCurrentIndex++;

	if (mCurrentIndex >= mSprites.size())
	{
		mCurrentIndex = 0;
	}
}

sf::Sprite& AnimatedSprite::CurrentSprite()
{
	return mSprites[mCurrentIndex];
}