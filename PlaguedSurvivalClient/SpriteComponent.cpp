#include "RoboCatClientPCH.hpp"


SpriteComponent::SpriteComponent(GameObject* inGameObject) :
	mGameObject(inGameObject)
{
	//and add yourself to the rendermanager...
	RenderManager::sInstance->AddComponent(this);
}

SpriteComponent::~SpriteComponent()
{
	//don't render me, I'm dead!
	RenderManager::sInstance->RemoveComponent(this);
}

void SpriteComponent::SetTexture(const TexturePtr& inTexture)
{
	auto tSize = inTexture->getSize();
	mSprite.setTexture(*inTexture);
	mSprite.setOrigin(tSize.x / 2, tSize.y / 2);
	mSprite.setScale(sf::Vector2f(1.f * mGameObject->GetScale(), 1.f * mGameObject->GetScale()));

	sf::FloatRect bounds = mSprite.getGlobalBounds();
	const Vector3 location = mGameObject->GetLocation();

	bounds.left = location.mX - (bounds.width * 0.5f);
	bounds.top = location.mY - (bounds.height * 0.5f);
	mGameObject->SetBounds(bounds);
}

sf::Sprite& SpriteComponent::GetSprite()
{
	// Update the sprite based on the game object stuff.
	auto pos = mGameObject->GetLocation();
	auto rot = mGameObject->GetRotation();
	mSprite.setPosition(pos.mX, pos.mY);
	mSprite.setRotation(rot);

	return mSprite;
}

sf::RectangleShape SpriteComponent::GetDebugBounds() const
{
	sf::RectangleShape shape;
	sf::FloatRect bounds = mGameObject->GetBounds();
	shape.setPosition(sf::Vector2f(bounds.left, bounds.top));
	shape.setSize(sf::Vector2f(bounds.width, bounds.height));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Green);
	shape.setOutlineThickness(1.f);

	return shape;
}
