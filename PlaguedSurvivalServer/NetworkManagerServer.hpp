class NetworkManagerServer : public NetworkManager
{
public:
	static NetworkManagerServer* sInstance;

	static bool	StaticInit(uint16_t inPort);

	virtual void ProcessPacket(InputMemoryBitStream& inInputStream, const SocketAddress& inFromAddress) override;
	virtual void HandleConnectionReset(const SocketAddress& inFromAddress) override;

	void SendOutgoingPackets();
	void CheckForDisconnects();

	void RegisterGameObject(const GameObjectPtr& inGameObject);
	inline GameObjectPtr RegisterAndReturn(GameObject* inGameObject);
	void UnregisterGameObject(GameObject* inGameObject);
	void SetStateDirty(int inNetworkId, uint32_t inDirtyState);

	void RespawnCats();

	ClientProxyPtr GetClientProxy(int inPlayerId) const;
	uint32_t GetClientProxyCount() const { return mPlayerIdToClientMap.size(); }
	void SendWinnerPacket(opt::PlayerId inPlayerId);

private:
	NetworkManagerServer();

	void HandlePacketFromNewClient(InputMemoryBitStream& inInputStream, const SocketAddress& inFromAddress);
	void ProcessPacket(const ClientProxyPtr& inClientProxy, InputMemoryBitStream& inInputStream);

	void SendWelcomePacket(const ClientProxyPtr& inClientProxy);
	void UpdateAllClients();

	void AddWorldStateToPacket(OutputMemoryBitStream& inOutputStream);
	void AddScoreBoardStateToPacket(OutputMemoryBitStream& inOutputStream);

	void SendStatePacketToClient(const ClientProxyPtr& inClientProxy);
	void WriteLastMoveTimestampIfDirty(OutputMemoryBitStream& inOutputStream, const ClientProxyPtr& inClientProxy);

	void HandleInputPacket(const ClientProxyPtr& inClientProxy, InputMemoryBitStream& inInputStream);

	void HandleClientDisconnected(const ClientProxyPtr& inClientProxy);

	int GetNewNetworkId();

	typedef unordered_map<opt::PlayerId, ClientProxyPtr> PlayerIdToClientMap;
	typedef unordered_map<SocketAddress, ClientProxyPtr> AddressToClientMap;

	AddressToClientMap mAddressToClientMap;
	PlayerIdToClientMap mPlayerIdToClientMap;

	opt::PlayerId mNewPlayerId;
	int mNewNetworkId;

	float mTimeOfLastSatePacket;
	float mTimeBetweenStatePackets;
	float mClientDisconnectTimeout;
};


inline GameObjectPtr NetworkManagerServer::RegisterAndReturn(GameObject* inGameObject)
{
	GameObjectPtr toRet(inGameObject);
	RegisterGameObject(toRet);
	return toRet;
}


