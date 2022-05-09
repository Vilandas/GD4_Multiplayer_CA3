class ReadWritePatterns
{
public:
	static constexpr uint32_t FLOAT_PRECISION = 100;
	static constexpr uint32_t DECIMAL_BITS = 8;

public:
	static void WriteLocation(OutputMemoryBitStream& inOutputStream, const Vector3& location);
	static Vector3 ReadLocation(InputMemoryBitStream& inInputStream);

	static void WriteLocationRounded(OutputMemoryBitStream& inOutputStream, const Vector3& location);
	static Vector3 ReadLocationRounded(InputMemoryBitStream& inInputStream);
};