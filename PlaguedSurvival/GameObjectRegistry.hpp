typedef GameObjectPtr(*GameObjectCreationFunc)();

class GameObjectRegistry
{
public:

	static void StaticInit();

	static std::unique_ptr<GameObjectRegistry> sInstance;

	void RegisterCreationFunction(ObjectTypes objectType, GameObjectCreationFunc inCreationFunction);

	GameObjectPtr CreateGameObject(ObjectTypes objectType);

private:

	GameObjectRegistry();

	unordered_map<opt::ObjectType, GameObjectCreationFunc> mNameToGameObjectCreationFunctionMap;
};

