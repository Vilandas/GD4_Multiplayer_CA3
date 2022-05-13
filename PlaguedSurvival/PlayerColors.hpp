/**
 * Vilandas Morrissey - D00218436
 */

enum class PlayerColors
{
	kWhite,
	kRed,
	kGreen,
	kBlue,
	kMagenta,
	kYellow,
	kCyan,
	kOrange,
	kApricot,
	kDeepPink,
	kPurple,
	kBrown,
	kMint,
	kDarkGreen,
	kTeal,
};

namespace ExtraColors
{
	static const Vector3 White(255, 255, 255);
	static const Vector3 Blue(0, 0, 255);
	static const Vector3 Red(255, 0, 0);
	static const Vector3 Green(0, 255, 0);
	static const Vector3 Magenta(255, 0, 255);
	static const Vector3 Yellow(255, 255, 0);
	static const Vector3 Cyan(0, 255, 255);

	static const Vector3 Orange(245, 130, 48);
	static const Vector3 Teal(0, 128, 128);
	static const Vector3 Purple(145, 30, 180);
	static const Vector3 DeepPink(255, 20, 147);
	static const Vector3 Apricot(255, 215, 180);
	static const Vector3 DarkGreen(0, 100, 0);
	static const Vector3 Brown(170, 110, 40);
	static const Vector3 Mint(170, 255, 195);

	inline Vector3 GetColor(PlayerColors playerColor)
	{
		switch (playerColor)
		{
		case PlayerColors::kWhite: return White;
		case PlayerColors::kBlue: return Blue;
		case PlayerColors::kRed: return Red;
		case PlayerColors::kGreen: return Green;
		case PlayerColors::kMagenta: return Magenta;
		case PlayerColors::kYellow: return Yellow;
		case PlayerColors::kCyan: return Cyan;
		case PlayerColors::kOrange: return Orange;
		case PlayerColors::kTeal: return Teal;
		case PlayerColors::kPurple: return Purple;
		case PlayerColors::kDeepPink: return DeepPink;
		case PlayerColors::kApricot: return Apricot;
		case PlayerColors::kDarkGreen: return DarkGreen;
		case PlayerColors::kBrown: return Brown;
		case PlayerColors::kMint: return Mint;
		}

		return White;
	}
}