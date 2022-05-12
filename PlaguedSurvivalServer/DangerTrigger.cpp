#include "RoboCatServerPCH.hpp"

/**
 * Vilandas Morrissey - D00218436
 */

std::unique_ptr<DangerTrigger> DangerTrigger::sInstance;

void DangerTrigger::StaticInit()
{
	sInstance.reset(new DangerTrigger());
}

DangerTrigger::DangerTrigger()
	: mDangers()
	, mDangerTime()
	, mDangersPerSecond(DEFAULT_DPS)
	, mDangerIncrementPerSecond(DEFAULT_IPS)
{
}

void DangerTrigger::Clear()
{
	mDangers.clear();
	mDangerTime = 0;
	mDangersPerSecond = DEFAULT_DPS;
	mDangerIncrementPerSecond = DEFAULT_IPS;
}

void DangerTrigger::AddDangerObject(Dangerous* danger)
{
	mDangers.emplace_back(danger);
}

void DangerTrigger::RemoveDangerObject(const Dangerous* danger)
{
	int idToDelete = dynamic_cast<const GameObject*>(danger)->GetNetworkId();
	for (auto iterator = mDangers.begin(); iterator != mDangers.end(); ++iterator)
	{
		int id = dynamic_cast<GameObject*>(*iterator)->GetNetworkId();
		if (id == idToDelete)
		{
			mDangers.erase(iterator);
			return;
		}
	}
	//mDangers.erase(std::find(mDangers.begin(), mDangers.end(), danger));
}

void DangerTrigger::Update(float inDeltaTime)
{
	if (mDangers.empty()) return;

	mDangerTime += inDeltaTime;

	while (mDangerTime >= 1)
	{
		mDangerTime -= 1;

		int dps = static_cast<int>(mDangersPerSecond);

		if (dps > mDangers.size())
		{
			dps = mDangers.size();
		}

		for (int i = 0; i < dps; i++)
		{
			const unsigned int chosenIndex = rand() % (mDangers.size());
			mDangers[chosenIndex]->Trigger();
		}
	}

	mDangersPerSecond += mDangerIncrementPerSecond * inDeltaTime;
}
