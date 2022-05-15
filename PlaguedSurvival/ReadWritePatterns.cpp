#include "RoboCatPCH.hpp"

void ReadWritePatterns::WriteLocation(OutputMemoryBitStream& inOutputStream, const Vector3& location) // 39 bits
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

void ReadWritePatterns::WriteLocationRounded(OutputMemoryBitStream& inOutputStream, const Vector3& location) //25 bits
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

void ReadWritePatterns::WriteColorRounded(OutputMemoryBitStream& inOutputStream, const Vector3& color) //24 bits
{
	WriteFloatRounded(inOutputStream, color.mX, 8);
	WriteFloatRounded(inOutputStream, color.mY, 8);
	WriteFloatRounded(inOutputStream, color.mZ, 8);
}

Vector3 ReadWritePatterns::ReadColorRounded(InputMemoryBitStream& inInputStream)
{
	Vector3 color = Vector3::Zero;

	color.mX = ReadFloatRounded(inInputStream, 8);
	color.mY = ReadFloatRounded(inInputStream, 8);
	color.mZ = ReadFloatRounded(inInputStream, 8);

	return color;
}

void ReadWritePatterns::WriteFloat(OutputMemoryBitStream& inOutputStream, float inValue, uint32_t bits) //8 + inBits
{
	const bool negative = inValue < 0;
	float delta = negative
		? inValue * -1
		: inValue;

	const auto value = static_cast<uint32_t>(delta);
	const auto decimal = static_cast<uint32_t>((delta - value) * FLOAT_PRECISION);

	inOutputStream.Write(negative);
	inOutputStream.Write(value, bits);
	inOutputStream.Write(decimal, DECIMAL_BITS);
}

float ReadWritePatterns::ReadFloat(InputMemoryBitStream& inInputStream, uint32_t bits)
{
	bool negative = false;
	uint32_t value = 0;
	uint32_t decimal = 0;
	inInputStream.Read(negative);
	inInputStream.Read(value, bits);
	inInputStream.Read(decimal, DECIMAL_BITS);

	float outValue = static_cast<float>(value) + (static_cast<float>(decimal) / FLOAT_PRECISION);

	if (negative)
	{
		outValue *= -1;
	}

	return outValue;
}

void ReadWritePatterns::WriteFloatRounded(OutputMemoryBitStream& inOutputStream, float inValue, uint32_t inBits) //1 + inBits
{
	const bool negative = inValue < 0;
	float delta = negative
		? inValue * -1
		: inValue;

	const auto value = static_cast<uint32_t>(delta);

	inOutputStream.Write(negative);
	inOutputStream.Write(value, inBits);
}

float ReadWritePatterns::ReadFloatRounded(InputMemoryBitStream& inInputStream, uint32_t inBits)
{
	bool negative = false;
	uint32_t value = 0;
	inInputStream.Read(negative);
	inInputStream.Read(value, inBits);

	auto outValue = static_cast<float>(value);

	if (negative)
	{
		outValue *= -1;
	}

	return outValue;
}

