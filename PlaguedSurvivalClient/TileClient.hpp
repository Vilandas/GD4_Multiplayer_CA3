class TileClient : public Tile
{
public:
	static GameObjectPtr StaticCreate() { return GameObjectPtr(new TileClient()); }

protected:
	TileClient();

private:
	SpriteComponentPtr mSpriteComponent;
};
