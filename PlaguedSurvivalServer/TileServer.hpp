class TileServer : public Tile, public Dangerous
{
public:
	static GameObjectPtr StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn(new TileServer()); }

	TileServer* GetAsTileServer() override { return this; }
	void HandleDying() override;
	void Trigger() override;

	void AddBelowTile(TileServer* tile);
	bool GetIsTop() const { return mIsTop; }
	void SetIsTop(bool isNew = false);
	void SetIsTop(const std::queue<TileServer*>& belowTiles);

	void SetLeftTile(TileServer* tile);
	void SetRightTile(TileServer* tile);
	void SetCollisionActive() override;

	uint32_t Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const override;

protected:
	TileServer();

	bool mIsTop;

	TileServer* mLeftTile;
	TileServer* mRightTile;
	std::queue<TileServer*> mBelowTiles;
};

