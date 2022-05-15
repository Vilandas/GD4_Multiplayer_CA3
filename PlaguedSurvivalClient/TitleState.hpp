class TitleState : public State
{
public:
	TitleState(StateStack& stack);
	void Draw() override;
	bool Update(float inDeltaTime) override;
	bool HandleEvent(const sf::Event& event) override;

private:
	sf::Sprite mBackgroundSprite;
	sf::Text mText;
	AnimatedSpriteArtistPtr mArtist;

	bool mShowText;
	sf::Time mTextEffectTime;
};