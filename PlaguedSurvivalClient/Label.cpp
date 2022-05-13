#include "RoboCatClientPCH.hpp"

namespace GUI
{
	Label::Label(const std::string& text)
		: m_text(text, *FontManager::sInstance->GetFont(Fonts::kCarlito), 16)
	{
	}

	bool Label::IsSelectable() const
	{
		return false;
	}

	std::string Label::GetText() const
	{
		return m_text.getString();
	}

	sf::Color Label::GetFillColor() const
	{
		return m_text.getFillColor();
	}

	void Label::SetText(const std::string& text)
	{
		m_text.setString(text);
	}

	void Label::SetFillColor(sf::Color color)
	{
		m_text.setFillColor(color);
	}

	void Label::SetCharacterSize(unsigned int size)
	{
		m_text.setCharacterSize(size);
	}

	void Label::CentreOriginText()
	{
		RoboMath::CentreOrigin(m_text);
	}

	void Label::HandleEvent(const sf::Event& event)
	{
	}

	void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(m_text, states);
	}
}

