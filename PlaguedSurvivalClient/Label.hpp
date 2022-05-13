namespace GUI
{
	class Label : public GUI::Component
	{
	public:
		typedef std::shared_ptr<Label> Ptr;
	public:
		Label(const std::string& text);
		bool IsSelectable() const override;
		std::string GetText() const;
		sf::Color GetFillColor() const;
		void SetText(const std::string& text);
		void SetFillColor(sf::Color color);
		void SetCharacterSize(unsigned int size);
		void CentreOriginText();
		void HandleEvent(const sf::Event& event) override;

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	private:
		sf::Text m_text;
	};
}