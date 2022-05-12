class TileClient : public Tile
{
public:
	static GameObjectPtr StaticCreate() { return GameObjectPtr(new TileClient()); }

	void SetTexture(Textures texture) override;
	void Read(InputMemoryBitStream& inInputStream) override;

protected:
	TileClient();

private:
	SpriteComponentPtr mSpriteComponent;
	SpriteComponentPtr mCrackSpriteComponent;
};
