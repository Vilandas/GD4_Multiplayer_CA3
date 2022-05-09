class Character : public GameObject
{
public:
	CLASS_IDENTIFICATION(static_cast<opt::ObjectType>(ObjectTypes::kCharacter), GameObject)

		enum CharacterReplicationState
	{
		CRS_Pose = 1 << 0,
		CRS_Color = 1 << 1,
		CRS_PlayerId = 1 << 2,

		CRS_AllState = CRS_Pose | CRS_Color | CRS_PlayerId
	};


	static GameObject* StaticCreate() { return new Character(); }

	uint32_t GetAllStateMask()	const override { return CRS_AllState; }
	Character* GetAsCharacter() override { return this; }
	void Update() override;

	void ProcessInput(float inDeltaTime, const InputState& inInputState);
	void SimulateMovement(float inDeltaTime);

	void ProcessCollisions();
	void ProcessCollisionsWithScreenWalls();

	void SetPlayerId(opt::PlayerId inPlayerId) { mPlayerId = inPlayerId; }
	opt::PlayerId GetPlayerId() const { return mPlayerId; }

	void SetVelocity(const sf::Vector2f& inVelocity) { mVelocity = inVelocity; }
	const sf::Vector2f& GetVelocity() const { return mVelocity; }

	uint32_t Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const override;

protected:
	Character();

private:
	void Accelerate(float inDeltaTime);
	void Decelerate(float inDeltaTime);
	void ApplyGravity(float inDeltaTime);
	void ValidateVelocity();

	void AdjustVelocityByThrust(float inDeltaTime);

	//float mLastMoveTimestamp;

protected:
	opt::PlayerId mPlayerId;
	float mHorizontalDirection;
	bool mIsJumping;
	float mAccelerationSpeed;
	float mDeceleration;
	float mGravity;
	sf::Vector2f mVelocity;
	sf::Vector2f mMaxVelocity;

	//float m_delta_time_in_seconds;
	//Camera& m_camera;
	//AnimatedSpriteArtist m_artist;
	//TextNode* m_name_display;
	//sf::Text m_arrow;

	float mJumpForce;
	float mCoyoteTime;
	float mAirTime;
	bool mCameraMoveConstraint;
	bool mIsCameraTarget;
};

typedef shared_ptr<Character> CharacterPtr;

