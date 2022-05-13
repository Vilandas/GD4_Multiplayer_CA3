/**
 * Vilandas Morrissey - D00218436
 */

class AnimatedSpriteArtist : public sf::Drawable, public sf::Transformable
{
public:
	AnimatedSpriteArtist(const std::vector<AnimationData>& data);
	~AnimatedSpriteArtist() override;

	void UpdateCurrent(float inDeltaTime);

	AnimatedSprite CurrentSpriteState() const;
	sf::FloatRect GetBoundingRect() const;
	sf::FloatRect GetLargestBounds() const;
	sf::FloatRect GetSmallestBounds() const;
	void ChangeState(int index);
	void Pause();
	void Play();
	void Flipped(bool flipped);

	void SetColor(sf::Color color);

private:
	AnimatedSprite& CurrentSpriteState();
	void CalculateLargestAndSmallest();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
	std::vector<AnimatedSprite> mSpriteStates;
	int mCurrentSpriteIndex;
	bool mFlipped;

	int mLargestBoundsIndex;
	int mSmallestBoundsIndex;
};

typedef shared_ptr<AnimatedSpriteArtist> AnimatedSpriteArtistPtr;