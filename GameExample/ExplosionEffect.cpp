#include "ExplosionEffect.h"



ExplosionEffect::ExplosionEffect()
{
	stateTime = 0;

	TexturePacker p = TexturePacker(&SharedTextures::BattleCityTexture, "../Resources/battlecity.xml");

	explosionAnimation.AddRegion(p.GetRegion("explosion_effect"));

	SetRegion(*explosionAnimation.GetKeyAnimation());
}


ExplosionEffect::~ExplosionEffect()
{
}

void ExplosionEffect::Render(SpriteBatch* batch)
{
	if (stateTime < 0) return;
	batch->Draw(*this);
}

void ExplosionEffect::Update(float dt)
{
	if (stateTime < 0) return;

	if (stateTime < EFFECTLIVETIME)
	{
		SetRegion(*explosionAnimation.Next(dt));
		stateTime += dt;
	}
	else
	{
		stateTime = -1;
	}

}

void ExplosionEffect::Play()
{
	stateTime = 0;
}

std::vector<ExplosionEffect> ExplosionEffectCollector:: mEffects;
