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

void Item::OnNetworkRead(InputMemoryBitStream & inInputStream, uint32_t dirtyState)
{
}

void Item::OnNetworkDestroy()
{
}
