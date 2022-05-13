class State
{
public:
	typedef std::unique_ptr<State> Ptr;

public:
	State(StateStack& stack);
	virtual ~State();
	virtual void Draw() = 0;
	virtual bool Update(float inDeltaTime) = 0;
	virtual bool HandleEvent(const sf::Event& event) = 0;
	virtual void OnActivate();
	virtual void OnDestroy();

protected:
	void RequestStackPush(StateId stateId);
	void RequestStackPop();
	void RequestStackClear();

private:
	StateStack* mStack;
};