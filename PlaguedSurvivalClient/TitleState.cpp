#include "RoboCatClientPCH.hpp"

TitleState::TitleState(StateStack& stack)
	: State(stack)
	//, m_artist(Table[static_cast<int>(PlatformerCharacterType::kDoc)].m_animation_data.ToVector(), *context.textures)
	, mShowText(true)
	, mTextEffectTime(sf::Time::Zero)
{
	mBackgroundSprite.setTexture(*TextureManager::sInstance->GetTexture(Textures::kTitleScreen));
	//m_artist.setPosition(380.f, 380.f);
	mText.setFont(*FontManager::sInstance->GetFont(Fonts::kCarlito));
	mText.setString("Press any key to continue");
	RoboMath::CentreOrigin(mText);
	mText.setPosition(WindowManager::sInstance->getView().getSize() / 2.f);
}

void TitleState::Draw()
{
	sf::RenderWindow& window = *WindowManager::sInstance;
	window.draw(mBackgroundSprite);
	//window.draw(m_artist);

	if (mShowText)
	{
		window.draw(mText);
	}
}

bool TitleState::Update(float inDeltaTime)
{
	//m_artist.UpdateCurrent(inDeltaTime);
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