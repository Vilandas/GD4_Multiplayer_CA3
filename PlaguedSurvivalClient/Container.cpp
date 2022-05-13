#include "RoboCatClientPCH.hpp"

namespace GUI
{
	Container::Container() :
		mWindow(*WindowManager::sInstance),
		mSelectedChild(-1)
	{
	}

	void Container::DeactivateAllExcept(const Component::Ptr& exception) const
	{
		for (const auto& child : mChildren)
		{
			if (child != exception)
			{
				child->Deactivate();
			}
		}
	}

	void Container::DeactivateAll() const
	{
		for (const auto& child : mChildren)
		{
			child->Deactivate();
		}
	}

	bool Container::Pack(const Component::Ptr& component)
	{
		component->SetParent(this);
		mChildren.emplace_back(component);
		if (!HasSelection() && component->IsSelectable())
		{
			Select(mChildren.size() - 1);
		}

		return true;
	}

	void Container::Unpack(const Component::Ptr& component)
	{
		mChildren.erase(std::remove(mChildren.begin(), mChildren.end(), component), mChildren.end());
	}

	bool Container::IsSelectable() const
	{
		return false;
	}

	void Container::HandleEvent(const sf::Event& event)
	{
		if (HasSelection() && mChildren[mSelectedChild]->IsActive())
		{
			mChildren[mSelectedChild]->HandleEvent(event);
		}

		if (event.type == sf::Event::MouseMoved)
		{
			for (size_t i = 0; i < mChildren.size(); i++)
			{
				const sf::Vector2f mouse = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow), mWindow.getView());
				const auto child = mChildren[i];
				if (child->GetBoundingRect().contains(mouse.x, mouse.y))
				{
					Select(i);
				}
			}
		}
		else if (event.type == sf::Event::MouseButtonReleased)
		{
			if (event.mouseButton.button == sf::Mouse::Left && HasSelection())
			{
				const sf::Vector2f mouse = mWindow.mapPixelToCoords(sf::Mouse::getPosition(mWindow), mWindow.getView());
				if (mChildren[mSelectedChild]->GetBoundingRect().contains(mouse.x, mouse.y))
				{
					mChildren[mSelectedChild]->Activate();
				}
			}
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::Up)
			{
				SelectPrevious();
			}
			else if (event.key.code == sf::Keyboard::S || event.key.code == sf::Keyboard::Down)
			{
				SelectNext();
			}
			else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space)
			{
				if (HasSelection())
				{
					mChildren[mSelectedChild]->Activate();
				}
			}
		}
	}

	void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		for (const Component::Ptr& child : mChildren)
		{
			target.draw(*child, states);
		}

	}

	bool Container::HasSelection() const
	{
		return mSelectedChild >= 0;
	}

	void Container::Select(std::size_t index)
	{
		if (index < mChildren.size() && mChildren[index]->IsSelectable())
		{
			if (HasSelection())
			{
				mChildren[mSelectedChild]->Deselect();
			}
			mChildren[index]->Select();
			mSelectedChild = index;
		}
	}

	void Container::SelectNext()
	{
		if (!HasSelection())
		{
			return;
		}
		//Search for the next component that is selectable and wrap around if necessary
		int next = mSelectedChild;
		do
		{
			next = (next + 1) % mChildren.size();
		} while (!mChildren[next]->IsSelectable());

		Select(next);
	}

	void Container::SelectPrevious()
	{
		if (!HasSelection())
		{
			return;
		}
		//Search for the next component that is selectable and wrap around if necessary
		int prev = mSelectedChild;
		do
		{
			prev = (prev + mChildren.size() - 1) % mChildren.size();
		} while (!mChildren[prev]->IsSelectable());

		Select(prev);
	}
}
