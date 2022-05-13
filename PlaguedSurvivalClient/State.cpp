#include "RoboCatClientPCH.hpp"

State::State(StateStack& stack) :
	mStack(&stack)
{
}

State::~State()
{
}


void State::RequestStackPush(StateId stateId)
{
	mStack->PushState(stateId);
}

void State::RequestStackPop()
{
	mStack->PopState();
}

void State::RequestStackClear()
{
	mStack->ClearStates();
}

void State::OnActivate()
{

}

void State::OnDestroy()
{

}
