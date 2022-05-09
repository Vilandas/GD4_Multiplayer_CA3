#pragma once
class TileServer : public Tile
{
public:
	static GameObjectPtr StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn(new TileServer()); }
	void HandleDying() override;

	bool HandleCollisionWithCat(RoboCat* inCat) override;

protected:
	TileServer();

};

