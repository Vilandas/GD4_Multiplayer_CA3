class TitleState : public State
{
public:
	TitleState(StateStack& stack);
	virtual void Draw();
	virtual bool Update(float inDeltaTime);
	virtual bool HandleEvent(const sf::Event& event);

private:
	sf::Sprite mBackgroundSprite;
	sf::Text mText;
	AnimatedSpriteArtistPtr mArtist;

	bool mShowText;
	sf::Time mTextEffectTime;
};