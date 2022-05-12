class Server : public Engine
{
public:

	static bool StaticInit();

	virtual void DoFrame() override;

	virtual int Run();

	void HandleNewClient(const ClientProxyPtr& inClientProxy);
	void HandleLostClient(const ClientProxyPtr& inClientProxy);

	CharacterPtr GetCharacterForPlayer(opt::PlayerId inPlayerId);
	void SpawnCharacterForPlayer(opt::PlayerId inPlayerId);


private:
	Server();

	bool InitNetworkManager();
	void SetupWorld();

	bool mTilesActivated;
};

