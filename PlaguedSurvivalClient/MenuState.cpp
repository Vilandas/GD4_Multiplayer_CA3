#include "RoboCatClientPCH.hpp"

MenuState::MenuState(StateStack& stack)
	: State(stack)
	//, m_artist(Table[static_cast<int>(PlatformerCharacterType::kDoc)].m_animation_data.ToVector(), *context.textures)
{
	sf::Texture& texture = *TextureManager::sInstance->GetTexture(Textures::kTitleScreen);

	mBackgroundSprite.setTexture(texture);
	//m_artist.setPosition(380.f, 380.f);

	auto host_play_button = std::make_shared<GUI::Button>();
	host_play_button->setPosition(100, 300);
	host_play_button->SetText("Host");
	host_play_button->SetCallback([this]()
		{
			RequestStackPop();
			RequestStackPush(StateId::kHostGame);
		});

	auto join_play_button = std::make_shared<GUI::Button>();
	join_play_button->setPosition(100, 350);
	join_play_button->SetText("Join");
	join_play_button->SetCallback([this]()
		{
			RequestStackPop();
			RequestStackPush(StateId::kJoinGame);
		});

	auto exit_button = std::make_shared<GUI::Button>();
	exit_button->setPosition(100, 400);
	exit_button->SetText("Exit");
	exit_button->SetCallback([this]()
		{
			RequestStackPop();
		});

	mGuiContainer.Pack(host_play_button);
	mGuiContainer.Pack(join_play_button);
	mGuiContainer.Pack(exit_button);

	// Play menu theme
	//context.music->Play(MusicThemes::kMenuTheme);
}

void MenuState::Draw()
{
	sf::RenderWindow& window = *WindowManager::sInstance;
	window.setView(window.getDefaultView());
	window.draw(mBackgroundSprite);
	//window.draw(m_artist);
	window.draw(mGuiContainer);

}

bool MenuState::Update(float inDeltaTime)
{
	//m_artist.UpdateCurrent(dt);
	return true;
}

bool MenuState::HandleEvent(const sf::Event& event)
{
	mGuiContainer.HandleEvent(event);
	return false;
}