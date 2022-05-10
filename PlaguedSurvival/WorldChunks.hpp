/**
 * Vilandas Morrissey - D00218436
 */

class WorldChunks
{
public:
	static void StaticInit();

	static std::unique_ptr<WorldChunks> sInstance;


	void Clear();
	void AddToChunk(GameObject* gameObject, Layers layer);
	void RemoveFromChunk(GameObject* gameObject, Layers layer);

	void CheckCollision(float inDeltaTime, GameObject* gameObject, Layers layer, std::set<GameObject*>& collisions);

private:
	WorldChunks();

private:
	std::vector<std::unordered_map<Layers, std::vector<GameObject*>>> mChunks;
};
