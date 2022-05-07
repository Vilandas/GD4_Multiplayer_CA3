/*
* the world tracks all the live game objects. Failry inefficient for now, but not that much of a problem
*/
class World
{
public:

	static void StaticInit();

	static std::unique_ptr<World>	sInstance;

	void AddGameObject(const GameObjectPtr& inGameObject);
	void RemoveGameObject(const GameObjectPtr& inGameObject);

	void Update();

	const std::vector<GameObjectPtr>& GetGameObjects() const { return mGameObjects; }

private:

	World();
	int	GetIndexOfGameObject(GameObjectPtr inGameObject);

private:

	std::vector<GameObjectPtr> mGameObjects;
};