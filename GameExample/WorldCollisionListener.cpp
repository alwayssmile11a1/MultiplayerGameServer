#include "WorldCollisionListener.h"

void WorldCollisionListener::OnCollisionEnter(Body * bodyA, Body * bodyB, const Vector2 & CollisionDirection)
{
	switch (bodyA->categoryBits * bodyB->categoryBits)
	{
	case PLAYER_BIT*BULLET_BIT:
	{
		if (bodyA->categoryBits == BULLET_BIT)
		{
			Bullet* bullet = (Bullet*)(bodyA->GetExtra());
			bullet->FakeExplosion();
		}
		else
		{
			if (bodyB->categoryBits == BULLET_BIT)
			{
				Bullet* bullet = (Bullet*)(bodyB->GetExtra());
				bullet->FakeExplosion();
			}
		}
		break;
	}
	case BRICK_BIT*BULLET_BIT:
	{
		if (bodyA->categoryBits == BULLET_BIT) {
			Bullet* bullet = (Bullet*)(bodyA->GetExtra());
			bullet->FakeExplosion();
		}
		else
		{
			if (bodyB->categoryBits == BULLET_BIT)
			{
				Bullet* bullet = (Bullet*)(bodyB->GetExtra());
				bullet->FakeExplosion();
			}
		break;
	}
	case METAL_BIT*BULLET_BIT:
	{
		if (bodyA->categoryBits == BULLET_BIT)
		{
			Bullet* bullet = (Bullet*)(bodyA->GetExtra());
			bullet->FakeExplosion();
		}
		else
		{
			if (bodyB->categoryBits == BULLET_BIT)
			{
				Bullet* bullet = (Bullet*)(bodyB->GetExtra());
				bullet->FakeExplosion();
			}
		}
		break;
	}
	case BULLET_BIT*BOUND_BIT:
	{
		if (bodyA->categoryBits == BULLET_BIT)
		{
			Bullet* bullet = (Bullet*)(bodyA->GetExtra());
			bullet->FakeExplosion();
		}
		else
		{
			if (bodyB->categoryBits == BULLET_BIT)
			{
				Bullet* bullet = (Bullet*)(bodyB->GetExtra());
				bullet->FakeExplosion();
			}
		}
		break;
	}
	}
	}
}

void WorldCollisionListener::OnColliding(Body * bodyA, Body * BodyB, const Vector2 & collisionDirection)
{
}

void WorldCollisionListener::OnCollisionExit(Body * bodyA, Body * bodyB, const Vector2 & collisionDirection)
{
}

void WorldCollisionListener::OnSersorEnter(Body * bodyA, Body * bodyB)
{
}

void WorldCollisionListener::OnSersorOverlaying(Body * bodyA, Body * bodyB)
{
}

void WorldCollisionListener::OnSensorExit(Body * bodyA, Body * bodyB)
{
}
