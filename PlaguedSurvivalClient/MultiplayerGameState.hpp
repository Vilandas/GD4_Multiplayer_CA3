/**
 * Vilandas Morrissey - D00218436
 */

class MultiplayerGameState : public State
{
public:
	MultiplayerGameState(StateStack& stack, bool is_host);
	void Draw() override;
	bool Update(float inDeltaTime) override;
	bool HandleEvent(const sf::Event& event) override;


private:
	GUI::Container mLobbyGui;
	sf::Sprite mBackgroundSprite;
	sf::Text mLobbyText;
	sf::Text mTopNameText;
	sf::Text mTopGamesWonText;
	sf::Text mWaitingForHostText;

	bool mHost;
	bool mLobby;
};