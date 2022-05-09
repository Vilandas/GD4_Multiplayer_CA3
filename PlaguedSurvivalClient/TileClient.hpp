class TileClient : public Tile
{
public:
	static GameObjectPtr StaticCreate() { return GameObjectPtr(new TileClient()); }

	void SetTexture(Textures texture) override;

protected:
	TileClient();

private:
	SpriteComponentPtr mSpriteComponent;
};
