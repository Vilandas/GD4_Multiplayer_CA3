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
	GUI::Container m_lobby_gui;
	sf::Sprite m_background_sprite;
	sf::Text m_lobby_text;
	sf::Text m_waiting_for_host_text;

	bool m_host;
	bool m_lobby;
};