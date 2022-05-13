#include "RoboCatClientPCH.hpp"

/**
 * Vilandas Morrissey - D00218436
 */

//unsigned int GetGamesWonFromFile()
//{
//	{
//		//Try to open existing file game_data.txt
//		std::ifstream input_file("game_data.txt");
//		opt::GamesWon games_won;
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
	, m_lobby_gui()
	, m_host(is_host)
	, m_lobby(true)
{
	//m_background_sprite.setTexture(*TextureManager::sInstance->GetTexture(Textures::kTitleScreen));

	m_lobby_text.setFont(*FontManager::sInstance->GetFont(Fonts::kCarlito));
	m_lobby_text.setString("Lobby");
	m_lobby_text.setCharacterSize(35);
	m_lobby_text.setFillColor(sf::Color::White);
	RoboMath::CentreOrigin(m_lobby_text);
	m_lobby_text.setPosition(960, 200);

	m_waiting_for_host_text.setFont(*FontManager::sInstance->GetFont(Fonts::kCarlito));
	m_waiting_for_host_text.setString("Waiting for host to start the game");
	m_waiting_for_host_text.setCharacterSize(24);
	m_waiting_for_host_text.setFillColor(sf::Color::White);
	RoboMath::CentreOrigin(m_waiting_for_host_text);
	m_waiting_for_host_text.setPosition(960, 800);

	if (m_host)
	{
		system("start ..\\Debug\\PlaguedSurvivalServer.exe 50000");

		auto start_button = std::make_shared<GUI::Button>();
		start_button->setPosition(860, 860);
		start_button->SetText("Start Game");
		start_button->SetCallback([this]()
			{
			});

		m_lobby_gui.Pack(start_button);
	}
	else
	{
	}
}

void MultiplayerGameState::Draw()
{
	//if (m_connected)
	//{
	//	//Show broadcast messages in default view
	//	m_window.setView(m_window.getDefaultView());

	//	if (m_lobby)
	//	{
	//		m_window.draw(m_background_sprite);

	//		sf::RectangleShape backgroundShape;
	//		backgroundShape.setFillColor(sf::Color(0, 0, 0, 200));
	//		backgroundShape.setSize(sf::Vector2f(600, 800));
	//		RoboMath::CentreOrigin(backgroundShape);
	//		backgroundShape.setPosition(960, 540);

	//		m_window.draw(backgroundShape);
	//		m_window.draw(m_lobby_gui);
	//		m_window.draw(m_lobby_text);

	//		for (const auto& pair : m_players)
	//		{
	//			const sf::Vector2f position = pair.second.m_name->getPosition();
	//			sf::Text text("Wins: " + std::to_string(pair.second.m_games_won), *FontManager::sInstance->GetFont(Fonts::kCarlito), 16);
	//			RoboMath::CentreOrigin(text);
	//			text.setFillColor(pair.second.m_name->GetFillColor());
	//			text.setPosition(position.x + 200, position.y);
	//			m_window.draw(text);
	//		}
	//	}
	//}
	//else
	//{
	//	m_window.draw(m_failed_connection_text);
	//}

	//m_window.draw(m_statistics_text);
}

bool MultiplayerGameState::Update(float inDeltaTime)
{

	Client::sInstance->ExternalDoFrame();

	NetworkManagerClient::sInstance->ProcessIncomingPackets();

	NetworkManagerClient::sInstance->SendOutgoingPackets();
	//UpdateStatistics(dt);

	////Connected to the Server: Handle all the network logic
	//if (m_connected)
	//{
	//	if (m_lobby)
	//	{
	//		UpdateLobby(dt);
	//	}
	//	else
	//	{
	//		UpdateGame(dt);
	//	}
	//}

	////Failed to connect and waited for more than 5 seconds: Back to menu
	//else if (m_failed_connection_clock.getElapsedTime() >= sf::seconds(5.f))
	//{
	//	RequestStackClear();
	//	RequestStackPush(StateID::kMenu);
	//}
	return true;
}

bool MultiplayerGameState::HandleEvent(const sf::Event& event)
{
	return true;
}
