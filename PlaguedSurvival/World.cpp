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
	mGameObjects[layer].push_back(inGameObject);
	inGameObject->SetIndexInWorld(mGameObjects[layer].size() - 1);
}


void World::RemoveGameObject(const GameObjectPtr& inGameObject)
{
	const Layers layer = inGameObject->GetLayer();
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
				RemoveGameObject(gameObject);
				gameObject->HandleDying();
				--j;
				--c;
			}
		}
	}
}
