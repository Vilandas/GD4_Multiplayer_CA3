class MenuState : public State
{
public:
	MenuState(StateStack& stack);
	virtual void Draw();
	virtual bool Update(float inDeltaTime);
	virtual bool HandleEvent(const sf::Event& event);

private:
	sf::Sprite mBackgroundSprite;
	//AnimatedSpriteArtist m_artist;
	GUI::Container mGuiContainer;
};
