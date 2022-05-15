class CharacterServer : public Character
{
public:
	static GameObjectPtr StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn(new CharacterServer()); }

	void HandleDying() override;
	void Update() override;

	void TakeDamage(opt::PlayerId inDamagingPlayerId);

protected:
	CharacterServer();

private:
	float mTimeOfNextShot;
	float mTimeBetweenShots;
};

