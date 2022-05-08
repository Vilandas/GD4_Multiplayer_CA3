class Tile : public GameObject
{
public:
	CLASS_IDENTIFICATION(static_cast<opt::ObjectType>(ObjectTypes::kTile), GameObject)

	enum TileReplicationState
	{
		TRS_Pose = 1 << 0,
		TRS_Health = 1 << 1,

		TRS_AllState = TRS_Pose | TRS_Health
	};

	static GameObject* StaticCreate() { return new Tile(); }

	uint32_t GetAllStateMask()	const override { return TRS_AllState; }
	Tile* GetAsTile() override { return this; }
	uint32_t Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const override;
	void Read(InputMemoryBitStream& inInputStream) override;

protected:
	Tile();

protected:
	int	mHealth;
	bool m_is_top;
	bool m_active_collision;

	Tile* m_left_tile;
	Tile* m_right_tile;
	std::queue<Tile*> m_below_tiles;
};

typedef shared_ptr<Tile> TilePtr;


//sf::Sprite m_sprite;
//sf::Sprite m_crack_sprite;