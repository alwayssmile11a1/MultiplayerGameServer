#include "WorldCollisionListener.h"

void WorldCollisionListener::OnCollisionEnter(Body * bodyA, Body * bodyB, const Vector2 & CollisionDirection)
{
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
