class Server : public Engine
{
public:

	static bool StaticInit();

	virtual void DoFrame() override;

	virtual int Run();

	void HandleNewClient(const ClientProxyPtr& inClientProxy);
	void HandleLostClient(const ClientProxyPtr& inClientProxy);

	CharacterPtr GetCharacterForPlayer(int inPlayerId);
	void SpawnCharacterForPlayer(int inPlayerId);


private:
	Server();

	bool InitNetworkManager();
	void SetupWorld();

};

