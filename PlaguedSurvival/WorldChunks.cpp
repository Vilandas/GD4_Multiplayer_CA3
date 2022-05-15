#include "RoboCatPCH.hpp"

/**
 * Vilandas Morrissey - D00218436
 */

std::unique_ptr<WorldChunks> WorldChunks::sInstance;

void WorldChunks::StaticInit()
{
	sInstance.reset(new WorldChunks());
}

WorldChunks::WorldChunks() :
	mChunks(WorldInfo::WORLD_CHUNKS)
{
}

void WorldChunks::Clear()
{
	for (auto& chunk : mChunks)
	{
		chunk.clear();
	}
}

void WorldChunks::AddToChunk(GameObject* gameObject, Layers layer)
{
	const size_t index = static_cast<size_t>(std::floorf(gameObject->GetLocationV2().x / WorldInfo::WIDTH_PER_CHUNK));
	mChunks[index][layer].emplace_back(gameObject);
}

void WorldChunks::RemoveFromChunk(GameObject* gameObject, Layers layer)
{
	const size_t index = static_cast<size_t>(std::floorf(gameObject->GetLocationV2().x / WorldInfo::WIDTH_PER_CHUNK));

	auto& chunk = mChunks[index][layer];
	chunk.erase(std::remove(chunk.begin(), chunk.end(), gameObject), chunk.end());
}

void WorldChunks::CheckCollision(float inDeltaTime, GameObject* gameObject, Layers layer, std::set<GameObject*>& collisions)
{
	size_t index = static_cast<size_t>(std::floorf(gameObject->GetLocationV2().x / WorldInfo::WIDTH_PER_CHUNK));
	size_t iterations = 3;

	if (index == 0)
	{
		iterations--;
	}
	else if (index >= mChunks.size() - 2)
	{
		iterations--;
		index = mChunks.size() - 2;
	}
	else
	{
		index--;
	}

	int counter = 0;

	for (size_t i = 0; i < iterations; i++)
	{
		for (const auto& otherObject : mChunks[index + i][layer])
		{
			counter++;
			gameObject->PredictCollisionWithObject(inDeltaTime, *otherObject, collisions);
		}
	}

	//LOG("Collisions Checked: %d", counter);
}