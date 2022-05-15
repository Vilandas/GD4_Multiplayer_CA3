namespace opt
{
	typedef uint32_t PlayerId;
	typedef sf::Uint8 ObjectType;
	typedef sf::Uint8 Texture;
	typedef sf::Uint8 Layer;


	constexpr uint32_t WorldWidthBits = 13;
	constexpr uint32_t WorldHeightBits = 12;

	constexpr uint32_t TextureBits = 6;
	constexpr uint32_t LayerBits = 4;

	constexpr uint32_t NameLength = 10;
}
