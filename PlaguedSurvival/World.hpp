/*
* the world tracks all the live game objects. Fairly inefficient for now, but not that much of a problem
*/
class World
{
public:
	static void StaticInit();

	static std::unique_ptr<World> sInstance;

	void AddGameObject(const GameObjectPtr& inGameObject);
	void AddGameObject(const GameObjectPtr& inGameObject, Layers layer);
	void RemoveGameObject(const GameObjectPtr& inGameObject);
	void RemoveGameObject(const GameObjectPtr& inGameObject, Layers layer);
	void SwapGameObjectLayer(const GameObjectPtr& inGameObject, Layers fromLayer, Layers toLayer);
	void SwapGameObjectLayer(const GameObject* inGameObject, Layers fromLayer, Layers toLayer);

	void Update();

	const std::vector<GameObjectPtr>& GetGameObjectsInLayer(const Layers layer) const { return mGameObjects.at(layer); }

private:
	World();
	//int	GetIndexOfGameObject(GameObjectPtr inGameObject);

private:
	std::unordered_map<Layers, std::vector<GameObjectPtr>> mGameObjects;
};