#ifndef BODY_H
#define BODY_H


#include"..\math\Vector2.h"
#include "GameObject.h"
#include<string>

#include <map>

class CollisionInfo
{
public:
	//No means this body haven't been checked collision
	//Enter means OnCollisionEnter or OnSensorEnter
	//On means OnColliding or OnSensorOverlaying
	//Exit means ...
	enum CollisionType {No, Enter, On, Exit };

public:
	int sensorState;

	int collisionState;

	CollisionInfo()
	{
		sensorState = CollisionType::No;
		collisionState = CollisionType::No;
	}

};

class Body: public GameObject
{
public: 
	//Static: can not apply velocity 
	//Kinematic: can be affected by velocity, but not gravity and linear drag
	//Dynamic: normal body
    enum BodyType { Static, Kinematic, Dynamic };

private:
	Vector2 _Position;
	Vector2 _Size;
	Vector2 _Velocity;
	Vector2 _LinearDrag;
	Vector2 _TotalVelocity;
	float _Mass; //the mass of body
	
	BodyType _BodyType;
	
	std::string _ID; //The id of this body
	
	int id;

	bool _IsSensor;

	bool _IsActived;

	GameObject* _Extra; //Extra information for this body
				 //can be anything
	
	//Go to the next position
	//this function was done by multiplying its velocity and deltatime 
	void Next(float dt, bool moveX, bool moveY);

	void CalculateActualVelocity(float dt, float gravity);
	const Vector2& GetTotalVelocity() const;
	
	Vector2 _PreviousPosition;

	std::map<Body*, CollisionInfo*> _CollisionPairStates;


private: //friend class
	friend class Collision;
	friend class World;
	friend class SpaceDivisionQuadTree;

public:
	//The categoryBits flag can be thought of as the fixture saying 'I am a ...'
	//usually just one Bit
	//NOTE:the value should be the power of two, such as: 1,2,4,8,16 ( 0x0001, 0x0002, 0x0004, 0x0008, 0x0010)
	unsigned long categoryBits;

	//the maskBits is like saying 'I will collide with a ...'
	unsigned long maskBits;
	//The idea was acquired from: http://www.iforce2d.net/b2dtut/collision-filtering
public:

	Body();
	Body(float x, float y, float width, float height, float vx, float vy);
	~Body();

	void SetVelocity(float vx, float vy);
	void SetSize(float width, float height);
	void SetPosition(float x, float y);
	void SetMass(float mass);
	void SetBodyType(BodyType type);
	//from 0-> infinity
	void SetLinearDrag(float xDrag, float yDrag);
	void SetID(const std::string &id);

	const Vector2& GetVelocity() const;
	const Vector2& GetSize() const;
	const Vector2& GetPosition() const;
	float GetMass() const;
	Body::BodyType GetBodyType();
	const std::string& GetID() const;

	void IsSensor(bool triggered);
	bool IsSensor() const;

	void IsActived(bool actived);
	bool IsActived() const;

	void PutExtra(GameObject* anything);
	GameObject* GetExtra();

	
};

#endif