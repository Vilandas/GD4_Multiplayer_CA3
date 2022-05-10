class CharacterClient : public Character
{
public:
	static GameObjectPtr StaticCreate() { return GameObjectPtr(new CharacterClient()); }

	virtual void Update();
	virtual void HandleDying() override;

	virtual void Read(InputMemoryBitStream& inInputStream) override;

	void DoClientSidePredictionAfterReplicationForLocalCharacter(uint32_t inReadState);
	void DoClientSidePredictionAfterReplicationForRemoteCharacter(uint32_t inReadState);

protected:
	CharacterClient();


private:
	void InterpolateClientSidePrediction(const Vector3& inOldLocation, const sf::Vector2f& inOldVelocity, bool inIsForRemoteCharacter);
	float mTimeLocationBecameOutOfSync;
	float mTimeVelocityBecameOutOfSync;

	SpriteComponentPtr mSpriteComponent;
};
