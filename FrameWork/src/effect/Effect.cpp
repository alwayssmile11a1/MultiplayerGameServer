#include "Effect.h"



Effect::Effect()
{
	_StateTime = -1;
	_LiveTime = 0;
}


Effect::~Effect()
{
}

Effect::Effect(const Animation& ani)
{
	_StateTime = -1;
	_LiveTime = 0;
	_Animation = ani;
	SetRegion(*_Animation.GetKeyAnimation());
	SetSize(_Animation.GetKeyAnimation()->GetRectSize().x, _Animation.GetKeyAnimation()->GetRectSize().y);
}

void Effect::Render(SpriteBatch* batch)
{
	if (_StateTime == -1) return;
	batch->Draw(*this);
}

void Effect::Update(float dt)
{
	if (_StateTime == -1) return;

	if (_StateTime < _LiveTime)
	{
		SetRegion(*_Animation.Next(dt));
		_StateTime += dt;
	}
	else
	{
		_StateTime = -1;
	}

}

void Effect::SetLiveTime(float liveTime)
{
	_LiveTime = liveTime;
}

void Effect::Play()
{
	_StateTime = 0;
}