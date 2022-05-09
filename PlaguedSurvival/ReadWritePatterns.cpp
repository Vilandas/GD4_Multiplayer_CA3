#include "RoboCatPCH.hpp"

void ReadWritePatterns::WriteLocation(OutputMemoryBitStream& inOutputStream, const Vector3& location)
{
	const int xPos = static_cast<int>(location.mX);
	const int xDecimal = static_cast<int>((location.mX - xPos) * FLOAT_PRECISION);

	const int yPos = static_cast<int>(location.mY);
	const int yDecimal = static_cast<int>((location.mY - yPos) * FLOAT_PRECISION);

	inOutputStream.Write(xPos, opt::WorldWidthBits);
	inOutputStream.Write(xDecimal, DECIMAL_BITS);
	inOutputStream.Write(yPos, opt::WorldHeightBits);
	inOutputStream.Write(yDecimal, DECIMAL_BITS);
}

Vector3 ReadWritePatterns::ReadLocation(InputMemoryBitStream& inInputStream)
{
	int xPos = 0;
	int xDecimal = 0;
	inInputStream.Read(xPos, opt::WorldWidthBits);
	inInputStream.Read(xDecimal, DECIMAL_BITS);

	int yPos = 0;
	int yDecimal = 0;
	inInputStream.Read(yPos, opt::WorldHeightBits);
	inInputStream.Read(yDecimal, DECIMAL_BITS);

	return {
		static_cast<float>(xPos) + (static_cast<float>(xDecimal) / FLOAT_PRECISION),
		static_cast<float>(yPos) + (static_cast<float>(yDecimal) / FLOAT_PRECISION),
		0
	};
}

void ReadWritePatterns::WriteLocationRounded(OutputMemoryBitStream& inOutputStream, const Vector3& location)
{
	int xPos = static_cast<int>(location.mX);
	int yPos = static_cast<int>(location.mY);

	inOutputStream.Write(xPos, opt::WorldWidthBits);
	inOutputStream.Write(yPos, opt::WorldHeightBits);
}

Vector3 ReadWritePatterns::ReadLocationRounded(InputMemoryBitStream& inInputStream)
{
	int xPos = 0;
	int yPos = 0;
	inInputStream.Read(xPos, opt::WorldWidthBits);
	inInputStream.Read(yPos, opt::WorldHeightBits);

	return {
		static_cast<float>(xPos),
		static_cast<float>(yPos),
		0
	};
}