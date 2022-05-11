#pragma once
class TileServer : public Tile
{
public:
	static GameObjectPtr StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn(new TileServer()); }
	void HandleDying() override;

	void Trigger() override;
	void SetIsTop(bool isNew) override;

	bool HandleCollisionWithCharacter(Character* inCharacter) override;

protected:
	TileServer();

};

