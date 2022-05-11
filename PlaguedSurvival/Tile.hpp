class Tile : public GameObject, public Dangerous
{
public:
	CLASS_IDENTIFICATION(static_cast<opt::ObjectType>(ObjectTypes::kTile), GameObject)

	enum TileReplicationState
	{
		TRS_Pose = 1 << 0,
		TRS_Health = 1 << 1,
		TRS_Texture = 1 << 2,
		TRS_State = 1 << 3,

		TRS_AllState = TRS_Pose | TRS_Health | TRS_Texture | TRS_State
	};

	static GameObject* StaticCreate() { return new Tile(); }

	void Trigger() override;

	uint32_t GetAllStateMask()	const override { return TRS_AllState; }
	Tile* GetAsTile() override { return this; }

	uint32_t Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const override;
	void Read(InputMemoryBitStream& inInputStream) override;

	virtual void SetTexture(Textures texture) { mTexture = texture; }

	void AddBelowTile(Tile* tile);
	bool GetIsTop() const { return mIsTop; }
	virtual void SetIsTop(bool isNew = false);
	void SetIsTop(const std::queue<Tile*>& belowTiles);
	void SetActiveCollision();

	void SetLeftTile(Tile* tile);
	void SetRightTile(Tile* tile);

protected:
	Tile();

protected:
	int mHealth;
	bool mIsTop;
	bool mActiveCollision;
	Textures mTexture;

	Tile* mLeftTile;
	Tile* mRightTile;
	std::queue<Tile*> mBelowTiles;
};

typedef shared_ptr<Tile> TilePtr;


//sf::Sprite mSprite;
//sf::Sprite m_crack_sprite;