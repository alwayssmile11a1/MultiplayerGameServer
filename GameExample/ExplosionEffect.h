#pragma once

#include "HanabiSprite.h"
#include "SharedTextures.h"
#include "HanabiConsole.h"

#define EFFECTLIVETIME 0.2
class ExplosionEffect :public Sprite
{
private:
	Animation explosionAnimation;

	float stateTime;

public:
	ExplosionEffect();
	~ExplosionEffect();

	void Update(float dt);

	void Render(SpriteBatch *batch);

	void Play();

	bool IsPlaying() { return stateTime > 0; }
};


class ExplosionEffectCollector
{
private:
	static std::vector<ExplosionEffect> mEffects;

public:

	static void PlayEffect(const Vector2& position)
	{
		for (std::vector<ExplosionEffect>::iterator effect = mEffects.begin(); effect != mEffects.end(); ++effect)
		{
			if (!effect->IsPlaying())
			{
				effect->SetPosition(position.x, position.y);
				effect->Play();
				return;
			}
		}

		//create new
		ExplosionEffect explosionEffect;
		explosionEffect.SetPosition(position.x, position.y);
		explosionEffect.Play();
		mEffects.push_back(explosionEffect);
	}

	static void Render(SpriteBatch* batch)
	{
		for (std::vector<ExplosionEffect>::iterator effect = mEffects.begin(); effect != mEffects.end(); ++effect)
		{
			effect->Render(batch);
		}
	}

	static void Update(float dt)
	{
		for (std::vector<ExplosionEffect>::iterator effect = mEffects.begin(); effect != mEffects.end(); ++effect)
		{
			effect->Update(dt);
		}
	}
};