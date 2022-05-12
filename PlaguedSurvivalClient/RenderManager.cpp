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

	for (SpriteComponent* c : mComponents)
	{
		if (c->GetGameObject().GetLayer() == Layers::kActivePlatforms)
		{
			WindowManager::sInstance->draw(c->GetDebugBounds());
		}
	}
}

void RenderManager::Render()
{
	//
	// Clear the back buffer
	//
	WindowManager::sInstance->clear(sf::Color(150, 150, 150, 255));

	RenderManager::sInstance->RenderComponents();

	HUD::sInstance->Render();

	//
	// Present our back buffer to our front buffer
	//
	WindowManager::sInstance->display();

}
