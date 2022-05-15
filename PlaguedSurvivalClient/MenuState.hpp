class MenuState : public State
{
public:
	MenuState(StateStack& stack);
	void Draw() override;
	bool Update(float inDeltaTime) override;
	bool HandleEvent(const sf::Event& event) override;

private:
	sf::Sprite mBackgroundSprite;
	AnimatedSpriteArtistPtr mArtist;
	GUI::Container mGuiContainer;
};
