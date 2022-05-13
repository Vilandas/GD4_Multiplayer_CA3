namespace GUI
{
	class Container : public Component
	{
	public:
		typedef std::shared_ptr<Container> Ptr;

	public:
		Container();
		void DeactivateAllExcept(const Component::Ptr& exception) const;
		void DeactivateAll() const;
		bool Pack(const Component::Ptr& component);
		void Unpack(const Component::Ptr& component);

		bool IsSelectable() const override;
		void HandleEvent(const sf::Event& event) override;

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
		bool HasSelection() const;
		void Select(std::size_t index);
		void SelectNext();
		void SelectPrevious();

	private:
		sf::RenderWindow& mWindow;
		std::vector<Component::Ptr> mChildren;
		int mSelectedChild;
	};
}