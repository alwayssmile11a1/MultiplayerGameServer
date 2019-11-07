#pragma once

#include "ClientNetworkManager.h"
#include "HanabiWorld.h"
#include "CollisionBit.h"
#include "Bullet.h"

class WorldCollisionListener: public WorldContactListener
{
	virtual void OnCollisionEnter(Body * bodyA, Body * bodyB, const Vector2 & CollisionDirection) override;
	virtual void OnColliding(Body * bodyA, Body * BodyB, const Vector2 & collisionDirection) override;
	virtual void OnCollisionExit(Body * bodyA, Body * bodyB, const Vector2 & collisionDirection) override;
	virtual void OnSersorEnter(Body * bodyA, Body * bodyB) override;
	virtual void OnSersorOverlaying(Body * bodyA, Body * bodyB) override;
	virtual void OnSensorExit(Body * bodyA, Body * bodyB) override;
};