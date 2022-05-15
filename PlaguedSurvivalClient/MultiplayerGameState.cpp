#include "RoboCatClientPCH.hpp"

/**
 * Vilandas Morrissey - D00218436
 */

MultiplayerGameState::MultiplayerGameState(StateStack& stack, bool is_host)
	: State(stack)
	, mLobbyGui()
	, mHost(is_host)
	, mLobby(true)
{
	mBackgroundSprite.setTexture(*TextureManager::sInstance->GetTexture(Textures::kTitleScreen));

	mLobbyText.setFont(*FontManager::sInstance->GetFont(Fonts::kCarlito));
	mLobbyText.setString("Lobby");
	mLobbyText.setCharacterSize(40);
	mLobbyText.setFillColor(sf::Color::White);
	RoboMath::CentreOrigin(mLobbyText);
	mLobbyText.setPosition(960, 200);


	mTopNameText.setFont(*FontManager::sInstance->GetFont(Fonts::kCarlito));
	mTopNameText.setString("Name");
	mTopNameText.setCharacterSize(32);
	mTopNameText.setFillColor(sf::Color::White);
	mTopNameText.setPosition(740, 230);

	mTopGamesWonText.setFont(*FontManager::sInstance->GetFont(Fonts::kCarlito));
	mTopGamesWonText.setString("Games Won");
	mTopGamesWonText.setCharacterSize(30);
	mTopGamesWonText.setFillColor(sf::Color::White);
	mTopGamesWonText.setPosition(1050, 230);


	mWaitingForHostText.setFont(*FontManager::sInstance->GetFont(Fonts::kCarlito));
	mWaitingForHostText.setString("Waiting for host to start the game");
	mWaitingForHostText.setCharacterSize(24);
	mWaitingForHostText.setFillColor(sf::Color::White);
	RoboMath::CentreOrigin(mWaitingForHostText);
	mWaitingForHostText.setPosition(960, 800);

	if (mHost)
	{
		system("start ..\\Debug\\PlaguedSurvivalServer.exe 50000");

		auto start_button = std::make_shared<GUI::Button>();
		start_button->setPosition(860, 860);
		start_button->SetText("Start Game");
		start_button->SetCallback([this]()
			{
				NetworkManagerClient::sInstance->SendStartGamePacket();
			});

		mLobbyGui.Pack(start_button);
	}
	else
	{
	}
}

void MultiplayerGameState::Draw()
{
	if (Client::sInstance->GameStarted())
	{
		
	}
	else
	{
		sf::RenderWindow& renderWindow = *WindowManager::sInstance;
		renderWindow.draw(mBackgroundSprite);

		sf::RectangleShape backgroundShape;
		backgroundShape.setFillColor(sf::Color(0, 0, 0, 200));
		backgroundShape.setSize(sf::Vector2f(600, 800));
		RoboMath::CentreOrigin(backgroundShape);
		backgroundShape.setPosition(960, 540);

		renderWindow.draw(backgroundShape);
		renderWindow.draw(mLobbyGui);
		renderWindow.draw(mLobbyText);
		renderWindow.draw(mTopNameText);
		renderWindow.draw(mTopGamesWonText);

		if (!mHost)
		{
			renderWindow.draw(mWaitingForHostText);
		}

		const FontPtr font = FontManager::sInstance->GetFont(Fonts::kCarlito);

		const vector<GameObjectPtr>& players = World::sInstance->GetGameObjectsInLayer(Layers::kPlayers);
		int i = 0;
		for (const GameObjectPtr& player : players)
		{
			const Character* character = player->GetAsCharacter();
			const auto entry = ScoreBoardManager::sInstance->GetEntry(character->GetPlayerId());

			if (entry != nullptr)
			{
				const Vector3 color = entry->GetColor();

				sf::Text textName = sf::Text(entry->GetPlayerName(), *font, 25);
				textName.setPosition(740, 280 + (25 * i));
				textName.setFillColor(sf::Color(color.mX, color.mY, color.mZ));
				renderWindow.draw(textName);

				sf::Text textScore = sf::Text(std::to_string(entry->GetScore()), *font, 25);
				textScore.setPosition(1100, 280 + (25 * i));
				textScore.setFillColor(textName.getFillColor());
				renderWindow.draw(textScore);
				i++;
			}
		}
	}
}

bool MultiplayerGameState::Update(float inDeltaTime)
{
	if (Client::sInstance->GameStarted())
	{
		Client::sInstance->ExternalDoFrame();
	}

	NetworkManagerClient::sInstance->ProcessIncomingPackets();

	NetworkManagerClient::sInstance->SendOutgoingPackets();

	return true;
}

bool MultiplayerGameState::HandleEvent(const sf::Event& event)
{
	mLobbyGui.HandleEvent(event);
	return true;
}
