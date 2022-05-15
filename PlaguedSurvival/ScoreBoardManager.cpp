#include "RoboCatPCH.hpp"

#include <fstream>

std::unique_ptr<ScoreBoardManager> ScoreBoardManager::sInstance;


void ScoreBoardManager::StaticInit()
{
	sInstance.reset(new ScoreBoardManager());
}

ScoreBoardManager::ScoreBoardManager()
{
	PlayerColors startColor = PlayerColors::kWhite;
	PlayerColors endColor = PlayerColors::kTeal;
	for (int color = static_cast<int>(startColor); color <= static_cast<int>(endColor); color++)
	{
		mDefaultColors.push_back(ExtraColors::GetColor(static_cast<PlayerColors>(color)));
	}
}

ScoreBoardManager::Entry::Entry(opt::PlayerId inPlayerId, const string& inPlayerName, const Vector3& inColor) :
	mColor(inColor),
	mPlayerId(inPlayerId),
	mPlayerName(inPlayerName),
	mScore()
{
	SetScore(0);
}

void ScoreBoardManager::Entry::SetScore(int32_t inScore)
{
	mScore = inScore;

	char buffer[256];
	snprintf(buffer, 256, "%s %i", mPlayerName.c_str(), mScore);
	mFormattedNameScore = string(buffer);

}


ScoreBoardManager::Entry* ScoreBoardManager::GetEntry(opt::PlayerId inPlayerId)
{
	for (Entry& entry : mEntries)
	{
		if (entry.GetPlayerId() == inPlayerId)
		{
			return &entry;
		}
	}

	return nullptr;
}

bool ScoreBoardManager::RemoveEntry(opt::PlayerId inPlayerId)
{
	for (auto eIt = mEntries.begin(), endIt = mEntries.end(); eIt != endIt; ++eIt)
	{
		if ((*eIt).GetPlayerId() == inPlayerId)
		{
			mEntries.erase(eIt);
			return true;
		}
	}

	return false;
}

void ScoreBoardManager::AddEntry(opt::PlayerId inPlayerId, const string& inPlayerName, uint32_t inGamesWon)
{
	//if this player id exists already, remove it first- it would be crazy to have two of the same id
	RemoveEntry(inPlayerId);

	mEntries.emplace_back(inPlayerId, inPlayerName, mDefaultColors[inPlayerId - 1 % mDefaultColors.size()]);
	IncScore(inPlayerId, inGamesWon);
}

void ScoreBoardManager::IncScore(opt::PlayerId inPlayerId, int inAmount)
{
	Entry* entry = GetEntry(inPlayerId);
	if (entry)
	{
		entry->SetScore(entry->GetScore() + inAmount);
	}
}



bool ScoreBoardManager::Write(OutputMemoryBitStream& inOutputStream) const
{
	int entryCount = mEntries.size();

	//we don't know our player names, so it's hard to check for remaining space in the packet...
	//not really a concern now though
	inOutputStream.Write(entryCount);
	for (const Entry& entry : mEntries)
	{
		entry.Write(inOutputStream);
	}

	return true;
}



bool ScoreBoardManager::Read(InputMemoryBitStream& inInputStream)
{
	int entryCount;
	inInputStream.Read(entryCount);
	//just replace everything that's here, it don't matter...
	mEntries.resize(entryCount);
	for (Entry& entry : mEntries)
	{
		entry.Read(inInputStream);
	}

	return true;
}


bool ScoreBoardManager::Entry::Write(OutputMemoryBitStream& inOutputStream) const
{
	bool didSucceed = true;

	inOutputStream.Write(mColor);
	inOutputStream.Write(mPlayerId, opt::PlayerIdBits);
	inOutputStream.Write(mPlayerName);
	inOutputStream.Write(mScore);

	return didSucceed;
}

bool ScoreBoardManager::Entry::Read(InputMemoryBitStream& inInputStream)
{
	bool didSucceed = true;

	inInputStream.Read(mColor);
	inInputStream.Read(mPlayerId, opt::PlayerIdBits);

	inInputStream.Read(mPlayerName);

	int score;
	inInputStream.Read(score);
	if (didSucceed)
	{
		SetScore(score);
	}


	return didSucceed;
}




