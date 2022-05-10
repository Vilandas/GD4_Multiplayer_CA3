/*
* the world tracks all the live game objects. Fairly inefficient for now, but not that much of a problem
*/
class World
{
public:
	static void StaticInit();

	static std::unique_ptr<World> sInstance;

	void AddGameObject(const GameObjectPtr& inGameObject);
	void RemoveGameObject(const GameObjectPtr& inGameObject);
	void SwapGameObjectLayer(const GameObjectPtr& inGameObject, Layers newLayer);
	void SwapGameObjectLayer(const GameObject& inGameObject, Layers newLayer);

	void Update();

	const std::vector<GameObjectPtr>& GetGameObjectsInLayer(const Layers layer) const { return mGameObjects.at(layer); }

private:
	World();
	int	GetIndexOfGameObject(GameObjectPtr inGameObject);

private:
	std::unordered_map<Layers, std::vector<GameObjectPtr>> mGameObjects;
};