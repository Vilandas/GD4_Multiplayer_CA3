#include "RoboCatPCH.hpp"
#include "Tile.hpp"

uint32_t Tile::Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const
{
	return GameObject::Write(inOutputStream, inDirtyState);
}

void Tile::Read(InputMemoryBitStream& inInputStream)
{
	GameObject::Read(inInputStream);
}

Tile::Tile()
{
}
