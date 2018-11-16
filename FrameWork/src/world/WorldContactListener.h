#ifndef WORLDCONTACTLISTENER_H
#define WORLDCONTACTLISTENER_H


#include "Body.h"
class WorldContactListener
{
public:
	WorldContactListener();
	~WorldContactListener();

	//Call everytime two object collide with each other. 
	//Occur at the same moment of update world function
	//Note that if it's possible, don't try to perform any of the physic things here because it may lead to wrong physic results later. 
	void virtual OnCollisionEnter(Body *bodyA, Body *bodyB, const Vector2 &CollisionDirection) = 0;

	void virtual OnColliding(Body*bodyA, Body* BodyB, const Vector2 &collisionDirection) = 0;

	void virtual OnCollisionExit(Body* bodyA, Body* bodyB, const Vector2 &collisionDirection) = 0;

	void virtual OnSersorEnter(Body *bodyA, Body *bodyB) = 0;

	void virtual OnSersorOverlaying(Body *bodyA, Body *bodyB) = 0;

	void virtual OnSensorExit(Body*bodyA, Body* bodyB) = 0;

};

#endif