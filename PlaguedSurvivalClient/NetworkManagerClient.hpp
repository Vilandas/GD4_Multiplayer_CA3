class NetworkManagerClient : public NetworkManager
{
	enum NetworkClientState
	{
		NCS_Uninitialized,
		NCS_SayingHello,
		NCS_Welcomed,
		NCS_StartGame,
	};

public:
	static NetworkManagerClient* sInstance;

	static void StaticInit(const SocketAddress& inServerAddress, const string& inName);

	void SendOutgoingPackets();

	virtual void ProcessPacket(InputMemoryBitStream& inInputStream, const SocketAddress& inFromAddress) override;

	const WeightedTimedMovingAverage& GetAvgRoundTripTime()	const { return mAvgRoundTripTime; }
	float GetRoundTripTime() const { return mAvgRoundTripTime.GetValue(); }
	opt::PlayerId GetPlayerId() const { return mPlayerId; }
	float GetLastMoveProcessedByServerTimestamp() const { return mLastMoveProcessedByServerTimestamp; }

	void SendStartGamePacket();

	void SetGamesWon(uint32_t gamesWon) { mGamesWon = gamesWon; }
	uint32_t GetGamesWon() const { return mGamesWon; }
	void Reset();

private:
	NetworkManagerClient();
	void Init(const SocketAddress& inServerAddress, const string& inName);

	void UpdateSayingHello();
	void SendHelloPacket();

	void HandleWelcomePacket(InputMemoryBitStream& inInputStream);
	void HandleStatePacket(InputMemoryBitStream& inInputStream);
	void ReadLastMoveProcessedOnServerTimestamp(InputMemoryBitStream& inInputStream);

	void HandleGameObjectState(InputMemoryBitStream& inInputStream);
	void HandleScoreBoardState(InputMemoryBitStream& inInputStream);

	void UpdateSendingInputPacket();
	void SendInputPacket();

	void DestroyGameObjectsInMap(const PlayedIdToGameObjectMap& inObjectsToDestroy);


	DeliveryNotificationManager mDeliveryNotificationManager;
	ReplicationManagerClient mReplicationManagerClient;

	SocketAddress mServerAddress;

	NetworkClientState mState;

	float mTimeOfLastHello;
	float mTimeOfLastInputPacket;

	string mName;
	opt::PlayerId mPlayerId;
	uint32_t mGamesWon;

	float mLastMoveProcessedByServerTimestamp;

	WeightedTimedMovingAverage	mAvgRoundTripTime;
	float mLastRoundTripTime;

};

