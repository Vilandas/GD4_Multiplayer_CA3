#include "RoboCatClientPCH.hpp"

namespace GUI
{
	Button::Button() :
		mSprite(*TextureManager::sInstance->GetTexture(Textures::kButtons)),
		mText("", *FontManager::sInstance->GetFont(Fonts::kCarlito), 16),
		mIsToggle(false)
	{
		ChangeTexture(ButtonType::Normal);
		sf::FloatRect bounds = mSprite.getLocalBounds();
		mText.setPosition(bounds.width / 2, bounds.height / 2);
	}

	void Button::SetCallback(Callback callback)
	{
		m_callback = std::move(callback);
	}

	void Button::SetText(const std::string& text)
	{
		mText.setString(text);
		RoboMath::CentreOrigin(mText);
	}

	void Button::SetToggle(bool flag)
	{
		mIsToggle = flag;
	}

	sf::FloatRect Button::GetBoundingRect() const
	{
		return GetWorldTransform().transformRect(mSprite.getGlobalBounds());
	}


	bool Button::IsSelectable() const
	{
		return true;
	}

	void Button::Select()
	{
		Component::Select();
		ChangeTexture(ButtonType::Selected);
	}

	void Button::Deselect()
	{
		Component::Deselect();
		ChangeTexture(ButtonType::Normal);
	}

	void Button::Activate()
	{
		Component::Activate();
		//If toggle then show button is pressed or toggled
		if (mIsToggle)
		{
			ChangeTexture(ButtonType::Pressed);
		}
		if (m_callback)
		{
			m_callback();
		}
		if (!mIsToggle)
		{
			Deactivate();
		}
		//m_sounds.Play(SoundEffect::kButton);
	}

	void Button::Deactivate()
	{
		Component::Deactivate();
		if (mIsToggle)
		{
			if (IsSelected())
			{
				ChangeTexture(ButtonType::Selected);
			}
			else
			{
				ChangeTexture(ButtonType::Normal);
			}
		}
	}

	void Button::HandleEvent(const sf::Event& event)
	{
	}

	void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(mSprite, states);
		target.draw(mText, states);
	}

	void Button::ChangeTexture(ButtonType buttonType)
	{
		sf::IntRect textureRect(0, 50 * static_cast<int>(buttonType), 200, 50);
		mSprite.setTextureRect(textureRect);
	}
}
