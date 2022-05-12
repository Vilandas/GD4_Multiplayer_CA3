#include "RoboCatServerPCH.hpp"
#include <iostream>



//uncomment this when you begin working on the server

bool Server::StaticInit()
{
	s_instance.reset(new Server());

	return true;
}

Server::Server() : mTilesActivated()
{

	GameObjectRegistry::sInstance->RegisterCreationFunction(ObjectTypes::kCharacter, CharacterServer::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction(ObjectTypes::kTile, TileServer::StaticCreate);

	InitNetworkManager();
	DangerTrigger::StaticInit();

	// Setup latency
	float latency = 0.0f;
	string latencyString = StringUtils::GetCommandLineArg(2);
	if (!latencyString.empty())
	{
		latency = stof(latencyString);
	}
	NetworkManagerServer::sInstance->SetSimulatedLatency(latency);
}


int Server::Run()
{
	SetupWorld();

	return Engine::Run();
}

bool Server::InitNetworkManager()
{
	string portString = StringUtils::GetCommandLineArg(1);
	uint16_t port = stoi(portString);

	return NetworkManagerServer::StaticInit(port);
}


namespace
{
	void CreateWorldTiles()
	{
		for (int i = 0; i < 195; i++)
		{
			GameObjectPtr gameObject = GameObjectRegistry::sInstance->CreateGameObject(ObjectTypes::kTile);
		}
	}

	void ActivateTiles()
	{
		const int map[][50] =
		{
			{0,0,0,0,0,0,2,0,0,0,0,0,0,0,1,3,0,0,1,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,4,5,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,1,5,5,5,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0},
			{0,0,2,2,2,2,2,2,2,2,2,2,2,3,0,0,1,5,5,5,5,5,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0},
			{1,2,5,5,5,5,5,5,5,5,5,5,5,6,0,0,4,5,5,5,5,5,5,3,0,0,0,0,0,0,0,0,0,0,0,0,1,2,2,2,2,5,2,2,2,2,2,2,2,3},
			{4,5,5,5,5,5,5,5,5,5,5,5,5,6,0,0,7,8,8,8,8,8,5,5,3,0,0,0,0,0,0,0,0,0,0,1,5,5,5,5,5,5,5,5,5,5,5,5,5,6},
			{7,8,8,8,8,8,8,8,8,8,8,8,8,9,0,0,0,0,0,0,0,0,4,5,6,0,0,0,0,0,0,0,0,1,2,5,8,8,8,8,8,8,8,8,8,8,8,8,8,6},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,8,5,3,0,0,0,0,0,0,0,4,5,9,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,8,2,2,2,2,2,2,2,8,8,0,0,0,0,0,0,0,0,0,0,0,0,8,8,9},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,2,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,5,3,0,0,0,5,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,2,0,0,4,0,0,0,0,4,6,0,0,0,0,0,0,0,0,0,5,5,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,8,0,0,4,0,0,0,0,4,6,0,0,0,0,0,0,0,0,0,0,8,0,0,0,0,0,0,5,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,4,0,0,0,0,4,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,8,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		};

		auto& gameObjects = World::sInstance->GetGameObjectsInLayer(Layers::kPlatforms);
		std::unordered_map<int, TileServer*> topTiles;
		int gameObjectIndex = gameObjects.size() - 1;

		for (int i = 0; i < 15; i++)
		{
			TileServer* lastTile = nullptr;
			for (int j = 0; j < 50; j++)
			{
				const int id = map[i][j];
				const bool skip = id == 0;

				if (!skip)
				{
					const Textures textureId = static_cast<Textures>(static_cast<int>(Textures::kDirt1) + id - 1);

					TileServer* tile = gameObjects[gameObjectIndex]->GetAsTileServer();

					tile->SetTexture(textureId);
					tile->SetScale(0.5f);

					tile->SetLocation(
						Vector3(
							32 + WorldInfo::TILE_SIZE + j * WorldInfo::TILE_SIZE,
							32 + (WorldInfo::WORLD_HEIGHT - WorldInfo::TILE_SIZE * 17) + i * WorldInfo::TILE_SIZE,
							0)
					);

					const auto result = topTiles.emplace(j, tile);
					if (result.second)
					{
						tile->SetIsTop();
					}
					else
					{
						topTiles[j]->AddBelowTile(tile);
					}

					if (lastTile != nullptr)
					{
						tile->SetLeftTile(lastTile);
						lastTile->SetRightTile(tile);
					}
					else //last_node was air so this node is exposed
					{
						tile->SetActiveCollision();
					}

					lastTile = tile;

					NetworkManagerServer::sInstance->SetStateDirty(tile->GetNetworkId(), tile->GetAllStateMask());
					gameObjectIndex--;
				}
				else
				{
					if (lastTile != nullptr)
					{
						lastTile->SetActiveCollision();
					}

					lastTile = nullptr;
				}

			}
		}
	}
}


void Server::SetupWorld()
{
	CreateWorldTiles();
}

void Server::DoFrame()
{
	NetworkManagerServer::sInstance->ProcessIncomingPackets();

	NetworkManagerServer::sInstance->CheckForDisconnects();

	NetworkManagerServer::sInstance->RespawnCats();

	Engine::DoFrame();

	DangerTrigger::sInstance->Update(Timing::sInstance.GetDeltaTime());

	NetworkManagerServer::sInstance->SendOutgoingPackets();

	if (!mTilesActivated)
	{
		mTilesActivated = true;
		ActivateTiles();
	}
}

void Server::HandleNewClient(const ClientProxyPtr& inClientProxy)
{
	int playerId = inClientProxy->GetPlayerId();

	ScoreBoardManager::sInstance->AddEntry(playerId, inClientProxy->GetName());
	SpawnCharacterForPlayer(playerId);
}

void Server::SpawnCharacterForPlayer(opt::PlayerId inPlayerId)
{
	CharacterPtr character = std::static_pointer_cast<Character>(GameObjectRegistry::sInstance->CreateGameObject(ObjectTypes::kCharacter));
	character->SetColor(ScoreBoardManager::sInstance->GetEntry(inPlayerId)->GetColor());
	character->SetPlayerId(inPlayerId);
	//gotta pick a better spawn location than this...
	character->SetLocation(Vector3(600.f - static_cast<float>(inPlayerId), 200.f, 0.f));
}

void Server::HandleLostClient(const ClientProxyPtr& inClientProxy)
{
	//kill client's cat
	//remove client from scoreboard
	int playerId = inClientProxy->GetPlayerId();

	ScoreBoardManager::sInstance->RemoveEntry(playerId);
	CharacterPtr character = GetCharacterForPlayer(playerId);
	if (character)
	{
		character->SetDoesWantToDie(true);
	}
}

CharacterPtr Server::GetCharacterForPlayer(opt::PlayerId inPlayerId)
{
	//run through the objects till we find the cat...
	//it would be nice if we kept a pointer to the cat on the clientproxy
	//but then we'd have to clean it up when the cat died, etc.
	//this will work for now until it's a perf issue
	const auto& players = World::sInstance->GetGameObjectsInLayer(Layers::kPlayers);
	for (const auto& gameObject : players)
	{
		Character* character = gameObject->GetAsCharacter();
		if (character && character->GetPlayerId() == inPlayerId)
		{
			return std::static_pointer_cast<Character>(gameObject);
		}
	}

	return nullptr;

}
