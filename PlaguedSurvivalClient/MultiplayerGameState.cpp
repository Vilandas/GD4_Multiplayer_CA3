#include "RoboCatClientPCH.hpp"

/**
 * Vilandas Morrissey - D00218436
 */

//unsigned int GetGamesWonFromFile()
//{
//	{
//		//Try to open existing file game_data.txt
//		std::ifstream input_file("game_data.txt");
//		uint32_t games_won;
//		if (input_file >> games_won)
//		{
//			return games_won;
//		}
//	}
//
//	//If open/read failed, create a new file
//	std::ofstream output_file("game_data.txt");
//	output_file << 0;
//	return 0;
//}

MultiplayerGameState::MultiplayerGameState(StateStack& stack, bool is_host)
	: State(stack)
	, mLobbyGui()
	, m_host(is_host)
	, mLobby(true)
{
	//mBackgroundSprite.setTexture(*TextureManager::sInstance->GetTexture(Textures::kTitleScreen));

	mLobbyText.setFont(*FontManager::sInstance->GetFont(Fonts::kCarlito));
	mLobbyText.setString("Lobby");
	mLobbyText.setCharacterSize(35);
	mLobbyText.setFillColor(sf::Color::White);
	RoboMath::CentreOrigin(mLobbyText);
	mLobbyText.setPosition(960, 200);

	mWaitingForHostText.setFont(*FontManager::sInstance->GetFont(Fonts::kCarlito));
	mWaitingForHostText.setString("Waiting for host to start the game");
	mWaitingForHostText.setCharacterSize(24);
	mWaitingForHostText.setFillColor(sf::Color::White);
	RoboMath::CentreOrigin(mWaitingForHostText);
	mWaitingForHostText.setPosition(960, 800);

	if (m_host)
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

				sf::Text textName = sf::Text(entry->GetPlayerName(), *font);
				textName.setPosition(760, 220 + (30 * i));
				textName.setFillColor(sf::Color(color.mX, color.mY, color.mZ));
				renderWindow.draw(textName);

				sf::Text textScore = sf::Text(std::to_string(entry->GetScore()), *font);
				textScore.setPosition(960, 220 + (30 * i));
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
