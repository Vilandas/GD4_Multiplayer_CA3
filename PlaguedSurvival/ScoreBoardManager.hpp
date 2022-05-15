class ScoreBoardManager
{
public:

	static void StaticInit();
	static std::unique_ptr< ScoreBoardManager >	sInstance;

	class Entry
	{
	public:
		Entry() {};

		Entry(opt::PlayerId inPlayerID, const string& inPlayerName, const Vector3& inColor);

		const Vector3& GetColor() const { return mColor; }
		sf::Color GetSfColor() const { return sf::Color(mColor.mX, mColor.mY, mColor.mZ); }
		opt::PlayerId GetPlayerId() const { return mPlayerId; }
		const string& GetPlayerName() const { return mPlayerName; }
		const string& GetFormattedNameScore() const { return mFormattedNameScore; }
		int	GetScore() const { return mScore; }

		void SetScore(int inScore);

		bool Write(OutputMemoryBitStream& inOutputStream) const;
		bool Read(InputMemoryBitStream& inInputStream);
		static uint32_t	GetSerializedSize();

	private:
		Vector3	mColor;

		opt::PlayerId mPlayerId;
		string mPlayerName;

		int	mScore;

		string mFormattedNameScore;
	};

	Entry* GetEntry(opt::PlayerId inPlayerId);
	bool RemoveEntry(opt::PlayerId inPlayerId);
	void AddEntry(opt::PlayerId inPlayerId, const string& inPlayerName, uint32_t inGamesWon);
	void IncScore(opt::PlayerId inPlayerId, int inAmount);

	bool Write(OutputMemoryBitStream& inOutputStream) const;
	bool Read(InputMemoryBitStream& inInputStream);

	const vector< Entry >& GetEntries()	const { return mEntries; }

	void SetWinnerId(opt::PlayerId id) { mWinnerId = id; }
	opt::PlayerId GetWinnerId() const { return mWinnerId; }

private:

	ScoreBoardManager();

	vector<Entry> mEntries;

	vector<Vector3> mDefaultColors;
	opt::PlayerId mWinnerId;
};

