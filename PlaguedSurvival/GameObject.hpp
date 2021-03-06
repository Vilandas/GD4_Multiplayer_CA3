#define CLASS_IDENTIFICATION(in_code, in_class) \
enum {kClassId = in_code}; \
virtual opt::ObjectType GetClassId() const {return kClassId;} \
static GameObject* CreateInstance() { return static_cast<GameObject*>(new in_class());}\

class GameObject
{
//public:
//	struct GameObjectHash {
//		size_t operator()(const GameObject& gameObject) const
//		{
//			return std::hash<int>{}(gameObject.GetNetworkId());
//		}
//	};
public:
	CLASS_IDENTIFICATION(static_cast<opt::ObjectType>(ObjectTypes::kGameObject), GameObject)

	GameObject();
	virtual ~GameObject() {}

	virtual bool operator==(const GameObject& otherObject) const { return GetNetworkId() == otherObject.GetNetworkId(); }

	virtual Character* GetAsCharacter() { return nullptr; }
	virtual Tile* GetAsTile() { return nullptr; }
	virtual TileServer* GetAsTileServer() { return nullptr; }
	virtual sf::Vector2f GetVelocity() const { return {}; }

	virtual uint32_t GetAllStateMask() const { return 0; }

	Layers GetLayer() const { return mLayer; }
	virtual void SetLayer(const Layers layer) { mLayer = layer; }

	//return whether to keep processing collision
	virtual bool HandleCollisionWithCharacter(Character* inChar) { (void)inChar; return true; }

	virtual void Update();
	virtual void HandleDying() {}

	void SetIndexInWorld(int inIndex) { mIndexInWorld = inIndex; }
	int GetIndexInWorld() const { return mIndexInWorld; }

	void SetRotation(float inRotation);
	float GetRotation() const { return mRotation; }

	void SetScale(float inScale) { mScale = inScale; }
	float GetScale() const { return mScale; }


	sf::Vector2f GetLocationV2() const { return sf::Vector2f(mLocation.mX, mLocation.mY); }
	const Vector3& GetLocation() const { return mLocation; }
	void SetLocation(const Vector3& inLocation);

	float GetCollisionRadius() const { return mCollisionRadius; }
	void SetCollisionRadius(float inRadius) { mCollisionRadius = inRadius; }

	sf::FloatRect GetBounds() const { return mBounds; }
	void SetBounds(sf::FloatRect bounds) { mBounds = bounds; }

	Vector3	GetForwardVector() const;

	void SetColor(const Vector3& inColor) { mColor = inColor; }
	const Vector3& GetColor() const { return mColor; }

	bool DoesWantToDie() const { return mDoesWantToDie; }
	void SetDoesWantToDie(bool inWants) { mDoesWantToDie = inWants; }

	int	GetNetworkId() const { return mNetworkId; }
	void SetNetworkId(int inNetworkId);

	//void PredictCollisionsWithScene(GameObject& otherObject, std::set<GameObject*, GameObjectHash>& collisions);
	void PredictCollisionsWithChunks(float inDeltaTime, Layers layer, std::set<GameObject*>& collisions);
	void PredictCollisionWithObject(float inDeltaTime, GameObject& otherObject, std::set<GameObject*>& collisions);

	virtual uint32_t Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const { (void)inOutputStream; (void)inDirtyState; return 0; }
	virtual void Read(InputMemoryBitStream& inInputStream) { (void)inInputStream; }

private:
	Layers mLayer;
	Vector3	mLocation;
	Vector3	mColor;

	float mCollisionRadius;
	sf::FloatRect mBounds;

	float mRotation;
	float mScale;
	int mIndexInWorld;

	bool mDoesWantToDie;

	int	mNetworkId;
};

typedef shared_ptr<GameObject> GameObjectPtr;