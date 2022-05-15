#include "RoboCatPCH.hpp"



std::unique_ptr<World> World::sInstance;

void World::StaticInit()
{
	sInstance.reset(new World());
}

World::World()
{
	for (int i = 0; i < static_cast<int>(Layers::kLayerCount); i++)
	{
		mGameObjects[static_cast<Layers>(i)] = std::vector<GameObjectPtr>();
	}
}


void World::AddGameObject(const GameObjectPtr& inGameObject)
{
	const Layers layer = inGameObject->GetLayer();
	AddGameObject(inGameObject, layer);
}

void World::AddGameObject(const GameObjectPtr& inGameObject, Layers layer)
{
	mGameObjects[layer].push_back(inGameObject);
	inGameObject->SetIndexInWorld(mGameObjects[layer].size() - 1);
}

void World::RemoveGameObject(const GameObjectPtr& inGameObject)
{
	const Layers layer = inGameObject->GetLayer();
	RemoveGameObject(inGameObject, layer);
}

void World::RemoveGameObject(const GameObjectPtr& inGameObject, Layers layer)
{
	const int index = inGameObject->GetIndexInWorld();
	const int lastIndex = mGameObjects[layer].size() - 1;

	if (index != lastIndex)
	{
		mGameObjects[layer][index] = mGameObjects[layer][lastIndex];
		mGameObjects[layer][index]->SetIndexInWorld(index);
	}

	inGameObject->SetIndexInWorld(-1);

	mGameObjects[layer].pop_back();
}

void World::SwapGameObjectLayer(const GameObjectPtr& inGameObject, Layers fromLayer, Layers toLayer)
{
	RemoveGameObject(inGameObject, fromLayer);
	AddGameObject(inGameObject, toLayer);
}

void World::SwapGameObjectLayer(const GameObject* inGameObject, Layers fromLayer, Layers toLayer)
{
	SwapGameObjectLayer(GetGameObjectsInLayer(fromLayer)[inGameObject->GetIndexInWorld()], fromLayer, toLayer);
}

int World::GetAlivePlayerCount() const
{
	const vector<GameObjectPtr>& gameObjects = GetGameObjectsInLayer(Layers::kPlayers);

	int aliveCount = 0;
	for (const GameObjectPtr& gameObject : gameObjects)
	{
		if (gameObject->GetAsCharacter()->IsAlive())
		{
			aliveCount++;
		}
	}

	return aliveCount;
}

opt::PlayerId World::GetFirstAlivePlayerId() const
{
	const vector<GameObjectPtr>& gameObjects = GetGameObjectsInLayer(Layers::kPlayers);

	for (const GameObjectPtr& gameObject : gameObjects)
	{
		if (gameObject->GetAsCharacter()->IsAlive())
		{
			return gameObject->GetAsCharacter()->GetPlayerId();
		}
	}

	return 0;
}


void World::Update()
{
	//update all game objects- sometimes they want to die, so we need to tread carefully...

	for (int i = 0; i < static_cast<int>(Layers::kLayerCount); ++i)
	{
		auto layer = static_cast<Layers>(i);
		std::vector<GameObjectPtr>& list = mGameObjects[layer];

		for (int j = 0, c = list.size(); j < c; ++j)
		{
			GameObjectPtr gameObject = list[j];

			if (!gameObject->DoesWantToDie())
			{
				gameObject->Update();
			}

			//you might suddenly want to die after your update, so check again
			if (gameObject->DoesWantToDie())
			{
				if (gameObject->GetIndexInWorld() != -1)
				{
					RemoveGameObject(gameObject);
				}

				gameObject->HandleDying();
				--j;
				--c;
			}
		}
	}
}
