/**
 * Vilandas Morrissey - D00218436
 */

struct AnimationData
{
	Textures mTexture;
	size_t mFrameWidth;
	size_t mFrameHeight;
	size_t mFrames;
	float mTimePerFrame;
};

class AnimatedSprite
{
public:
	AnimatedSprite(const AnimationData& data, const sf::Texture& texture);

	void DrawCurrent(sf::RenderTarget& target, sf::RenderStates& states, bool flipped) const;
	void UpdateCurrent(float inDeltaTime);

	sf::Sprite CurrentSprite() const;
	sf::FloatRect GetBoundingRect();
	void Pause();
	void Play();
	void Reset();

	void SetColor(sf::Color color);

private:
	void Advance();
	sf::Sprite& CurrentSprite();

private:
	std::vector<sf::Sprite> mSprites;
	float mTimeSinceLastFrame;
	float mTimePerFrame;
	int mCurrentIndex;
	bool mLoop;
	bool mPaused;
};
