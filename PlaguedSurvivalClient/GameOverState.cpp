#include "RoboCatClientPCH.hpp"

GameOverState::GameOverState(StateStack& stack)
	: State(stack)
	, mGameOverText()
	, mElapsedTime(sf::Time::Zero)
{
	const sf::Font& font = *FontManager::sInstance->GetFont(Fonts::kCarlito);
	const sf::Vector2f windowSize(WindowManager::sInstance->getSize());

	const int winnerId = ScoreBoardManager::sInstance->GetWinnerId();

	if (winnerId == 0)
	{
		mGameOverText.setString("Nobody Wins!");
	}
	else
	{
		ScoreBoardManager::Entry& entry = *ScoreBoardManager::sInstance->GetEntry(winnerId);
		mGameOverText.setString(entry.GetPlayerName() + " Wins!");
		mGameOverText.setFillColor(entry.GetSfColor());
	}

	if (NetworkManagerClient::sInstance->GetPlayerId() == winnerId)
	{
		std::ofstream saveData;
		saveData.open("gameData.txt", std::fstream::out);
		NetworkManagerClient::sInstance->SetGamesWon(NetworkManagerClient::sInstance->GetGamesWon() + 1);

		saveData << NetworkManagerClient::sInstance->GetGamesWon();
		saveData.close();
	}

	mGameOverText.setFont(font);
	mGameOverText.setCharacterSize(70);
	RoboMath::CentreOrigin(mGameOverText);
	mGameOverText.setPosition(0.5f * windowSize.x, 0.4f * windowSize.y);

	Client::sInstance->EndGame();
}

void GameOverState::Draw()
{
	sf::RenderWindow& window = *WindowManager::sInstance;
	window.setView(window.getDefaultView());

	// Create dark, semitransparent background
	sf::RectangleShape backgroundShape;
	backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
	backgroundShape.setSize(window.getView().getSize());

	window.draw(backgroundShape);
	window.draw(mGameOverText);
}

bool GameOverState::Update(float inDeltaTime)
{
	// Show state for 3 seconds, after return to menu
	mElapsedTime += sf::seconds(inDeltaTime);
	if (mElapsedTime > sf::seconds(3))
	{
		Client::sInstance->SetShouldReset();
	}
	return false;
}

bool GameOverState::HandleEvent(const sf::Event&)
{
	return false;
}