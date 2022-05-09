class InputState
{
public:

	InputState() :
		mDesiredRightAmount(0),
		mDesiredLeftAmount(0),
		mDesiredForwardAmount(0),
		mDesiredBackAmount(0),
		mIsJumping(false)
	{}

	float GetDesiredHorizontalDelta() const { return mDesiredRightAmount - mDesiredLeftAmount; }
	float GetDesiredVerticalDelta() const { return mDesiredForwardAmount - mDesiredBackAmount; }
	bool IsJumping() const { return mIsJumping; }

	bool Write(OutputMemoryBitStream& inOutputStream) const;
	bool Read(InputMemoryBitStream& inInputStream);

private:
	friend class InputManager;

	float mDesiredRightAmount, mDesiredLeftAmount;
	float mDesiredForwardAmount, mDesiredBackAmount;
	bool mIsJumping;
};
