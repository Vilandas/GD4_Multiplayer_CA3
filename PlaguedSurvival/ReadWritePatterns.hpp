class ReadWritePatterns
{
public:
	static constexpr uint32_t FLOAT_PRECISION = 100;
	static constexpr uint32_t DECIMAL_BITS = 7;

public:
	static void WriteLocation(OutputMemoryBitStream& inOutputStream, const Vector3& location);
	static Vector3 ReadLocation(InputMemoryBitStream& inInputStream);

	static void WriteLocationRounded(OutputMemoryBitStream& inOutputStream, const Vector3& location);
	static Vector3 ReadLocationRounded(InputMemoryBitStream& inInputStream);


	static void WriteColorRounded(OutputMemoryBitStream& inOutputStream, const Vector3& color);
	static Vector3 ReadColorRounded(InputMemoryBitStream& inInputStream);


	static void WriteFloat(OutputMemoryBitStream& inOutputStream, float inValue, uint32_t bits);
	static float ReadFloat(InputMemoryBitStream& inInputStream, uint32_t bits);

	static void WriteFloatRounded(OutputMemoryBitStream& inOutputStream, float inValue, uint32_t inBits);
	static float ReadFloatRounded(InputMemoryBitStream& inInputStream, uint32_t inBits);
};