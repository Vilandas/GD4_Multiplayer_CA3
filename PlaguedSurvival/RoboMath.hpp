class Vector3
{
public:

	float mX, mY, mZ;

	Vector3(float x, float y, float z) :
		mX(x),
		mY(y),
		mZ(z)
	{}

	Vector3() :
		mX(0.0f),
		mY(0.0f),
		mZ(0.0f)
	{}

	void Set(float x, float y, float z)
	{
		mX = x;
		mY = y;
		mZ = z;
	}

	friend Vector3 operator+(const Vector3& inLeft, const Vector3& inRight)
	{
		return Vector3(inLeft.mX + inRight.mX, inLeft.mY + inRight.mY, inLeft.mZ + inRight.mZ);
	}

	friend Vector3 operator+(const Vector3& inLeft, const sf::Vector2f& inRight)
	{
		return Vector3(inLeft.mX + inRight.x, inLeft.mY + inRight.y, inLeft.mZ);
	}

	friend Vector3 operator+(const sf::Vector2f& inLeft, const Vector3& inRight)
	{
		return Vector3(inLeft.x + inRight.mX, inLeft.y + inRight.mY, inRight.mZ);
	}

	friend Vector3 operator-(const Vector3& inLeft, const Vector3& inRight)
	{
		return Vector3(inLeft.mX - inRight.mX, inLeft.mY - inRight.mY, inLeft.mZ - inRight.mZ);
	}

	// Component-wise multiplication
	friend Vector3 operator*(const Vector3& inLeft, const Vector3& inRight)
	{
		return Vector3(inLeft.mX * inRight.mX, inLeft.mY * inRight.mY, inLeft.mZ * inRight.mZ);
	}

	// Scalar multiply
	friend Vector3 operator*(float inScalar, const Vector3& inVec)
	{
		return Vector3(inVec.mX * inScalar, inVec.mY * inScalar, inVec.mZ * inScalar);
	}

	friend Vector3 operator*(const Vector3& inVec, float inScalar)
	{
		return Vector3(inVec.mX * inScalar, inVec.mY * inScalar, inVec.mZ * inScalar);
	}

	Vector3& operator*=(float inScalar)
	{
		mX *= inScalar;
		mY *= inScalar;
		mZ *= inScalar;
		return *this;
	}

	Vector3& operator+=(const Vector3& inRight)
	{
		mX += inRight.mX;
		mY += inRight.mY;
		mZ += inRight.mZ;
		return *this;
	}

	Vector3& operator+=(const sf::Vector2f& inRight)
	{
		mX += inRight.x;
		mY += inRight.y;;
		return *this;
	}

	Vector3& operator-=(const Vector3& inRight)
	{
		mX -= inRight.mX;
		mY -= inRight.mY;
		mZ -= inRight.mZ;
		return *this;
	}

	float Length()
	{
		return sqrtf(mX * mX + mY * mY + mZ * mZ);
	}

	float LengthSq()
	{
		return mX * mX + mY * mY + mZ * mZ;
	}

	float Length2D()
	{
		return sqrtf(mX * mX + mY * mY);
	}

	float LengthSq2D()
	{
		return mX * mX + mY * mY;
	}

	void Normalize()
	{
		float length = Length();
		mX /= length;
		mY /= length;
		mZ /= length;
	}

	void Normalize2D()
	{
		float length = Length2D();
		mX /= length;
		mY /= length;
	}

	friend float Dot(const Vector3& inLeft, const Vector3& inRight)
	{
		return (inLeft.mX * inRight.mX + inLeft.mY * inRight.mY + inLeft.mZ * inRight.mZ);
	}

	friend float Dot2D(const Vector3& inLeft, const Vector3& inRight)
	{
		return (inLeft.mX * inRight.mX + inLeft.mY * inRight.mY);
	}

	friend Vector3 Cross(const Vector3& inLeft, const Vector3& inRight)
	{
		Vector3 temp;
		temp.mX = inLeft.mY * inRight.mZ - inLeft.mZ * inRight.mY;
		temp.mY = inLeft.mZ * inRight.mX - inLeft.mX * inRight.mZ;
		temp.mZ = inLeft.mX * inRight.mY - inLeft.mY * inRight.mX;
		return temp;
	}

	friend Vector3 Lerp(const Vector3& inA, const Vector3& inB, float t)
	{
		return Vector3(inA + t * (inB - inA));
	}

	static const Vector3 Zero;
	static const Vector3 UnitX;
	static const Vector3 UnitY;
	static const Vector3 UnitZ;
};


class Quaternion
{
public:

	float mX, mY, mZ, mW;

};


template< int tValue, int tBits >
struct GetRequiredBitsHelper
{
	enum { Value = GetRequiredBitsHelper< (tValue >> 1), tBits + 1 >::Value };
};

template< int tBits >
struct GetRequiredBitsHelper< 0, tBits >
{
	enum { Value = tBits };
};

template< int tValue >
struct GetRequiredBits
{
	enum { Value = GetRequiredBitsHelper< tValue, 0 >::Value };
};

namespace RoboMath
{
	const float PI = 3.1415926535f;
	float GetRandomFloat();

	Vector3 GetRandomVector(const Vector3& inMin, const Vector3& inMax);

	inline bool	Is2DVectorEqual(const Vector3& inA, const Vector3& inB)
	{
		return (inA.mX == inB.mX && inA.mY == inB.mY);
	}

	inline float ToDegrees(float inRadians)
	{
		return inRadians * 180.0f / PI;
	}

	inline float ToRadians(float inDegrees)
	{
		return (inDegrees / 180.0f) * PI;
	}

	inline sf::Vector2f Lerp(const sf::Vector2f& inA, const sf::Vector2f& inB, float t)
	{
		return sf::Vector2f(inA + t * (inB - inA));
	}

	inline sf::Vector2f GetRectCenter(const sf::FloatRect& rect)
	{
		return { rect.left + rect.width / 2, rect.top + rect.height / 2 };
	}

	inline float Length(const sf::Vector2f& vector)
	{
		return sqrtf(powf(vector.x, 2) + powf(vector.y, 2));
	}

	inline void CentreOrigin(sf::Text& text)
	{
		const sf::FloatRect bounds = text.getLocalBounds();
		text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
	}

	inline void CentreOrigin(sf::RectangleShape& shape)
	{
		const sf::FloatRect bounds = shape.getLocalBounds();
		shape.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
	}

	inline void CentreOrigin(sf::Sprite& sprite)
	{
		const sf::FloatRect bounds = sprite.getLocalBounds();
		sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
	}

}

namespace Colors
{
	static const Vector3 Black(0.0f, 0.0f, 0.0f);
	static const Vector3 White(255.0f, 255.0f, 255.0f);
	static const Vector3 Red(255.0f, 0.0f, 0.0f);
	static const Vector3 Green(0.0f, 255.0f, 0.0f);
	static const Vector3 Blue(0.0f, 0.0f, 255.0f);
	static const Vector3 LightYellow(255.0f, 255.0f, 224.4f);
	static const Vector3 LightBlue(173.4f, 216.75f, 229.5f);
	static const Vector3 LightPink(255.0f, 181.05f, 193.8f);
	static const Vector3 LightGreen(142.8f, 237.15f, 142.8f);
}

