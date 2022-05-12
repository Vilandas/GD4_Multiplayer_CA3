class Tile : public GameObject
{
public:
	CLASS_IDENTIFICATION(static_cast<opt::ObjectType>(ObjectTypes::kTile), GameObject)

	enum TileReplicationState
	{
		TRS_Init = 1 << 0,
		TRS_Pose = 1 << 1,
		TRS_Health = 1 << 2,
		TRS_Texture = 1 << 3,
		TRS_State = 1 << 4,

		TRS_AllState = TRS_Pose | TRS_Health | TRS_Texture | TRS_State
	};

	static GameObject* StaticCreate() { return new Tile(); }

	void HandleDying() override;
	void SetLayer(const Layers layer) override;

	uint32_t GetAllStateMask()	const override { return TRS_AllState; }
	Tile* GetAsTile() override { return this; }

	virtual void SetTexture(Textures texture) { mTexture = texture; }

protected:
	Tile();

protected:
	uint32_t mHealth;
	Textures mTexture;
};

typedef shared_ptr<Tile> TilePtr;


//sf::Sprite mSprite;
//sf::Sprite m_crack_sprite;