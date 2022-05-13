#include "RoboCatClientPCH.hpp"

std::unique_ptr<StateStack> StateStack::sInstance;

void StateStack::StaticInit()
{
	sInstance.reset(new StateStack());
}

StateStack::StateStack()
{
	RegisterState<TitleState>(StateId::kTitle);
	RegisterState<MenuState>(StateId::kMenu);

	RegisterState<MultiplayerGameState>(StateId::kHostGame, true);
	RegisterState<MultiplayerGameState>(StateId::kJoinGame, false);

	RegisterState<GameOverState>(StateId::kGameOver);
}

void StateStack::Update(float inDeltaTime)
{
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->Update(inDeltaTime))
		{
			break;
		}
	}
	ApplyPendingChanges();

	if (IsEmpty())
	{
		Engine::sInstance->SetShouldKeepRunning(false);
	}
}

void StateStack::Draw()
{
	for (State::Ptr& state : mStack)
	{
		state->Draw();
	}
}

void StateStack::HandleEvent(const sf::Event& event)
{
	for (auto itr = mStack.rbegin(); itr != mStack.rend(); ++itr)
	{
		if (!(*itr)->HandleEvent(event))
		{
			break;
		}
	}
	ApplyPendingChanges();
}

void StateStack::PushState(StateId stateId)
{
	mPendingList.emplace_back(PendingChange(Action::Push, stateId));
}

void StateStack::PopState()
{
	mPendingList.emplace_back(PendingChange(Action::Pop));
}

void StateStack::ClearStates()
{
	mPendingList.emplace_back(PendingChange(Action::Clear));
}

bool StateStack::IsEmpty() const
{
	return mStack.empty();
}

State::Ptr StateStack::CreateState(StateId stateId)
{
	auto found = mStateFactory.find(stateId);
	assert(found != mStateFactory.end());
	return found->second();
}

void StateStack::ApplyPendingChanges()
{
	for (PendingChange change : mPendingList)
	{
		switch (change.action)
		{
		case Action::Push:
			mStack.emplace_back(CreateState(change.stateId));
			break;
		case Action::Pop:
			mStack.back()->OnDestroy();
			mStack.pop_back();
			if (!mStack.empty())
			{
				mStack.back()->OnActivate();
			}
			break;
		case Action::Clear:
			for (State::Ptr& state : mStack)
			{
				state->OnDestroy();
			}
			mStack.clear();
			break;
		}
	}
	mPendingList.clear();
}

StateStack::PendingChange::PendingChange(Action action, StateId stateId)
	: action(action)
	, stateId(stateId)
{
}

