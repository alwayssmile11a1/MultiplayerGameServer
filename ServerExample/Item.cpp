#include "Item.h"



Item::Item()
{
}


Item::~Item()
{
}

void Item::Render(SpriteBatch * batch)
{
	batch->Draw(mSprite);
}

uint32_t Item::OnNetworkWrite(OutputMemoryBitStream & inOutputStream, uint32_t inDirtyState) const
{
	return uint32_t();
}

void Item::OnNetworkDestroy()
{
}
