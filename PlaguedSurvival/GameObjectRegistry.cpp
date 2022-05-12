#include "RoboCatPCH.hpp"


std::unique_ptr<GameObjectRegistry> GameObjectRegistry::sInstance;

void GameObjectRegistry::StaticInit()
{
	sInstance.reset(new GameObjectRegistry());
}

GameObjectRegistry::GameObjectRegistry()
{
}

void GameObjectRegistry::RegisterCreationFunction(ObjectTypes objectType, GameObjectCreationFunc inCreationFunction)
{
	mNameToGameObjectCreationFunctionMap[static_cast<opt::ObjectType>(objectType)] = inCreationFunction;
}

GameObjectPtr GameObjectRegistry::CreateGameObject(ObjectTypes objectType)
{
	//no error checking- if the name isn't there, exception!
	GameObjectCreationFunc creationFunc = mNameToGameObjectCreationFunctionMap[static_cast<opt::ObjectType>(objectType)];

	GameObjectPtr gameObject = creationFunc();

	//should the registry depend on the world? this might be a little weird
	//perhaps you should ask the world to spawn things? for now it will be like this
	World::sInstance->AddGameObject(gameObject);

	return gameObject;
}
