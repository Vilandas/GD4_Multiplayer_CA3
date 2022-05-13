class StateStack : private sf::NonCopyable
{
public:
	enum class Action
	{
		Push,
		Pop,
		Clear
	};

public:
	static void StaticInit();

	static std::unique_ptr<StateStack> sInstance;

	explicit StateStack();
	template <typename T>
	void RegisterState(StateId stateId);
	template <typename T, typename Param1>
	void RegisterState(StateId stateId, Param1 arg1);
	void Update(float inDeltaTime);
	void Draw();
	void HandleEvent(const sf::Event& event);

	void PushState(StateId stateId);
	void PopState();
	void ClearStates();

	bool IsEmpty() const;

private:
	State::Ptr CreateState(StateId stateId);
	void ApplyPendingChanges();

private:
	struct PendingChange
	{
		explicit PendingChange(Action action, StateId stateId = StateId::kNone);
		Action action;
		StateId stateId;
	};

private:
	std::vector<State::Ptr> mStack;
	std::vector<PendingChange> mPendingList;
	std::map<StateId, std::function<State::Ptr()>> mStateFactory;
};

template <typename T>
void StateStack::RegisterState(StateId stateId)
{
	mStateFactory[stateId] = [this]()
	{
		return State::Ptr(new T(*this));
	};
}

template <typename T, typename Param1>
void StateStack::RegisterState(StateId stateId, Param1 arg1)
{
	mStateFactory[stateId] = [this, arg1]()
	{
		return State::Ptr(new T(*this, arg1));
	};
}