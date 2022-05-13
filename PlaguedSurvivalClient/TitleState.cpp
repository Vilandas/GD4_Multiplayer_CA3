#include "RoboCatClientPCH.hpp"

TitleState::TitleState(StateStack& stack)
	: State(stack)
	, mShowText(true)
	, mTextEffectTime(sf::Time::Zero)
{
	mBackgroundSprite.setTexture(*TextureManager::sInstance->GetTexture(Textures::kTitleScreen));
	mText.setFont(*FontManager::sInstance->GetFont(Fonts::kCarlito));
	mText.setString("Press any key to continue");
	RoboMath::CentreOrigin(mText);
	mText.setPosition(WindowManager::sInstance->getView().getSize() / 2.f);

	mArtist.reset(new AnimatedSpriteArtist(DataTables::CharacterData.ToVector()));
	mArtist->setPosition(380.f, 380.f);
}

void TitleState::Draw()
{
	sf::RenderWindow& window = *WindowManager::sInstance;
	window.draw(mBackgroundSprite);
	window.draw(*mArtist);

	if (mShowText)
	{
		window.draw(mText);
	}
}

bool TitleState::Update(float inDeltaTime)
{
	mArtist->UpdateCurrent(inDeltaTime);
	mTextEffectTime += sf::seconds(inDeltaTime);

	if (mTextEffectTime >= sf::seconds(0.5))
	{
		mShowText = !mShowText;
		mTextEffectTime = sf::Time::Zero;
	}
	return true;
}

bool TitleState::HandleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyReleased)
	{
		RequestStackPop();
		RequestStackPush(StateId::kMenu);
	}
	return true;
}