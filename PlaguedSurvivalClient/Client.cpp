#include "RoboCatClientPCH.hpp"

uint32_t GetGamesWonFromFile()
{
	{
		//Try to open existing file game_data.txt
		std::ifstream input_file("gameData.txt");
		uint32_t gamesWon;
		if (input_file >> gamesWon)
		{
			return gamesWon;
		}
	}

	//If open/read failed, create a new file
	std::ofstream outputFile("gameData.txt");
	outputFile << 0;
	return 0;
}

bool Client::StaticInit()
{
	StateStack::StaticInit();

	// Create the Client pointer first because it initializes SDL
	Client* client = new Client();
	InputManager::StaticInit();

	WindowManager::StaticInit();
	FontManager::StaticInit();
	TextureManager::StaticInit();
	MusicPlayer::StaticInit();
	RenderManager::StaticInit();

	HUD::StaticInit();

	sInstance.reset(client);

	return true;
}

void Client::Reset()
{
	World::StaticInit();
	WorldChunks::StaticInit();
	ScoreBoardManager::StaticInit();

	RenderManager::sInstance->Reset();
	HUD::StaticInit();

	NetworkManagerClient::sInstance->Reset();

	StateStack::sInstance->ClearStates();
	StateStack::sInstance->PushState(StateId::kMenu);
}

void Client::ExternalDoFrame()
{
	Engine::DoFrame();
}

Client::Client()
{
	GameObjectRegistry::sInstance->RegisterCreationFunction(ObjectTypes::kCharacter, CharacterClient::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction(ObjectTypes::kTile, TileClient::StaticCreate);

	string destination = StringUtils::GetCommandLineArg(1);
	string name = StringUtils::GetCommandLineArg(2);
	if (name.size() > opt::NameLength)
	{
		name = name.substr(0, opt::NameLength);
	}

	SocketAddressPtr serverAddress = SocketAddressFactory::CreateIPv4FromString(destination);

	NetworkManagerClient::StaticInit(*serverAddress, name);
	NetworkManagerClient::sInstance->SetGamesWon(GetGamesWonFromFile());

	StateStack::sInstance->PushState(StateId::kTitle);

	//NetworkManagerClient::sInstance->SetSimulatedLatency(0.0f);
}



void Client::DoFrame()
{
	if (ShouldReset())
	{
		mShouldReset = false;
		Reset();
	}

	InputManager::sInstance->Update();

	StateStack::sInstance->Update(Timing::sInstance.GetDeltaTime());

	RenderManager::sInstance->Render();
}

void Client::HandleEvent(sf::Event& p_event)
{
	switch (p_event.type)
	{
	case sf::Event::KeyPressed:
		InputManager::sInstance->HandleInput(EIA_Pressed, p_event.key.code);
		break;
	case sf::Event::KeyReleased:
		InputManager::sInstance->HandleInput(EIA_Released, p_event.key.code);
		break;
	default:
		break;
	}

	StateStack::sInstance->HandleEvent(p_event);
}

bool Client::PollEvent(sf::Event& p_event)
{
	return WindowManager::sInstance->pollEvent(p_event);
}


