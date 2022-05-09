class Tile : public GameObject
{
public:
	CLASS_IDENTIFICATION(static_cast<opt::ObjectType>(ObjectTypes::kTile), GameObject)

	enum TileReplicationState
	{
		TRS_Pose = 1 << 0,
		TRS_Health = 1 << 1,
		TRS_Texture = 1 << 2,

		TRS_AllState = TRS_Pose | TRS_Health | TRS_Texture
	};

	static GameObject* StaticCreate() { return new Tile(); }

	uint32_t GetAllStateMask()	const override { return TRS_AllState; }
	Tile* GetAsTile() override { return this; }

	uint32_t Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const override;
	void Read(InputMemoryBitStream& inInputStream) override;

	virtual void SetTexture(Textures texture) { mTexture = texture; }

	void AddBelowTile(Tile* tile);
	void SetIsTop(bool is_new = false);
	void SetIsTop(const std::queue<Tile*>& below_tiles);
	void SetActiveCollision();

	void SetLeftTile(Tile* tile);
	void SetRightTile(Tile* tile);

protected:
	Tile();

protected:
	uint32_t mHealth;
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