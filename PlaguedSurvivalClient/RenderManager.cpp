#include "RoboCatClientPCH.hpp"

std::unique_ptr<RenderManager> RenderManager::sInstance;

RenderManager::RenderManager()
{
	view.reset(sf::FloatRect(0, 0, 1920, 1080));
	WindowManager::sInstance->setView(view);
}


void RenderManager::StaticInit()
{
	sInstance.reset(new RenderManager());
}


void RenderManager::AddComponent(SpriteComponent* inComponent)
{
	mComponents.emplace_back(inComponent);
}

void RenderManager::RemoveComponent(SpriteComponent* inComponent)
{
	int index = GetComponentIndex(inComponent);

	if (index != -1)
	{
		int lastIndex = mComponents.size() - 1;
		if (index != lastIndex)
		{
			mComponents[index] = mComponents[lastIndex];
		}
		mComponents.pop_back();
	}
}

int RenderManager::GetComponentIndex(SpriteComponent* inComponent) const
{
	for (int i = 0, c = mComponents.size(); i < c; ++i)
	{
		if (mComponents[i] == inComponent)
		{
			return i;
		}
	}

	return -1;
}

void RenderManager::AddArtist(AnimatedSpriteArtist* inArtist, bool priorityRender)
{
	if (priorityRender)
	{
		mPriorityArtists.emplace_back(inArtist);
	}
	else mArtists.emplace_back(inArtist);
}

void RenderManager::RemoveArtist(AnimatedSpriteArtist* inArtist)
{
	int index = GetArtistIndex(inArtist, false);

	if (index == -1)
	{
		index = GetArtistIndex(inArtist, true);

		if (index != -1)
		{
			int lastIndex = mPriorityArtists.size() - 1;
			if (index != lastIndex)
			{
				mPriorityArtists[index] = mPriorityArtists[lastIndex];
			}
			mPriorityArtists.pop_back();
		}

		return;
	}

	if (index != -1)
	{
		int lastIndex = mArtists.size() - 1;
		if (index != lastIndex)
		{
			mArtists[index] = mArtists[lastIndex];
		}
		mArtists.pop_back();
	}
}

int RenderManager::GetArtistIndex(AnimatedSpriteArtist* inArtist, bool priorityRender) const
{
	const vector<AnimatedSpriteArtist*>& artists = priorityRender
		? mPriorityArtists
		: mArtists;

	for (int i = 0, c = artists.size(); i < c; ++i)
	{
		if (artists[i] == inArtist)
		{
			return i;
		}
	}

	return -1;
}

void RenderManager::AddText(sf::Text* inText)
{
	mExternalTexts.emplace_back(inText);
}

void RenderManager::RemoveText(sf::Text* inText)
{
	int index = GetTextIndex(inText);

	if (index != -1)
	{
		int lastIndex = mExternalTexts.size() - 1;
		if (index != lastIndex)
		{
			mExternalTexts[index] = mExternalTexts[lastIndex];
		}
		mExternalTexts.pop_back();
	}
}

int RenderManager::GetTextIndex(sf::Text* inText) const
{
	for (int i = 0, c = mExternalTexts.size(); i < c; ++i)
	{
		if (mExternalTexts[i] == inText)
		{
			return i;
		}
	}

	return -1;
}


//this part that renders the world is really a camera-
//in a more detailed engine, we'd have a list of cameras, and then render manager would
//render the cameras in order
void RenderManager::RenderComponents()
{
	//Get the logical viewport so we can pass this to the SpriteComponents when it's draw time
	for (SpriteComponent* c : mComponents)
	{	
		WindowManager::sInstance->draw(c->GetSprite());
	}

	for (const AnimatedSpriteArtist* artist : mArtists)
	{
		WindowManager::sInstance->draw(*artist);
	}

	for (const AnimatedSpriteArtist* artist : mPriorityArtists)
	{
		WindowManager::sInstance->draw(*artist);
	}

	for (const sf::Text* drawable : mExternalTexts)
	{
		WindowManager::sInstance->draw(*drawable);
	}

	//for (SpriteComponent* c : mComponents)
	//{
	//	if (c->GetGameObject().GetLayer() == Layers::kPlatforms)
	//	{
	//		WindowManager::sInstance->draw(c->GetDebugBounds());
	//	}
	//}
}

void RenderManager::Render()
{
	//
	// Clear the back buffer
	//
	WindowManager::sInstance->clear(sf::Color(150, 150, 150, 255));

	RenderManager::sInstance->RenderComponents();

	HUD::sInstance->Render();

	StateStack::sInstance->Draw();

	//
	// Present our back buffer to our front buffer
	//
	WindowManager::sInstance->display();
}
