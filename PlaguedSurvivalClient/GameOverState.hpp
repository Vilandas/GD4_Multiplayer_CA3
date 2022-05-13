class GameOverState : public State
{
public:
	GameOverState(StateStack& stack);

	void Draw() override;
	bool Update(float inDeltaTime) override;
	bool HandleEvent(const sf::Event& event) override;

private:
	sf::Text mGameOverText;
	sf::Time mElapsedTime;
};