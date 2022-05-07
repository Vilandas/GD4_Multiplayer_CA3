#pragma once
class TileServer : public Tile
{
public:
	static GameObjectPtr StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn(new TileServer()); }
	void HandleDying() override;
	virtual bool HandleCollisionWithCat(RoboCat* inCat) override;

protected:
	TileServer();
};

