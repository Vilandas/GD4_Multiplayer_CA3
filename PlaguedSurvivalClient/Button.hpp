namespace GUI
{
	class Button : public Component
	{
	public:
		typedef std::shared_ptr<Button> Ptr;
		typedef std::function<void()> Callback;

	public:
		Button();
		void SetCallback(Callback callback);
		void SetText(const std::string& text);
		void SetToggle(bool flag);

		sf::FloatRect GetBoundingRect() const override;
		bool IsSelectable() const override;
		void Select() override;
		void Deselect() override;
		void Activate() override;
		void Deactivate() override;
		void HandleEvent(const sf::Event& event) override;

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void ChangeTexture(ButtonType buttonType);

	private:
		Callback m_callback;
		sf::Sprite mSprite;
		sf::Text mText;
		bool mIsToggle;
	};
}