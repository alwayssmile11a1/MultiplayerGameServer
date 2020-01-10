#include "Item.h"



Item::Item()
{
}


Item::~Item()
{
}

void Item::Update(float dt)
{
	//WorldCollector::GetWorld('PS')->UpdateForBody(mMainBody, dt);
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
