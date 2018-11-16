#ifndef WORLD_H
#define WORLD_H

#include<vector>
#include "GameObject.h"
#include"..\drawable\SpriteBatch.h"
#include"Collision.h"
#include"WorldContactListener.h"
#include <algorithm>  
#include "BodyDef.h"
#include "..\collisionquadtree\QuadTree.h"
#include "../spacedivisionquadtree/SpaceDivisionQuadTree.h"

#define USECOLLISIONINFO 0 //using collision info is more precise for collision callback, but decreasing performance


// add body to this class and physic things will be handled
class World: public GameObject
{
private:
	
	float _Gravity;  
	std::vector<Body*> _Bodies; 	//list of all bodies. Using pointer like this may be better, I think. 
									//Note that we do not allocate new memory to this list, just hold the references to our objects

	WorldContactListener* _Listener;  //listen to the collision
	
	QuadTree* _QuadTree;			//Quadtree of this world

	Camera* _Cam;

	Collision collision;

	SpaceDivisionQuadTree *_SDQuadTree;

	

private:

	friend class Collision;

	void CheckCollision(Body* body1, Body* body2, const RECT &broadphaseRect, const RECT &staticRect, bool &moveX, bool &moveY, float dt);


public:
	//Constructor and destructor
	World();
	World(float gravity);
	~World();

	//World(const World &world);
	//World& operator=(const World &world);

	//if camera is set, collisionQuadTree will be used
	void SetCamera(Camera *cam);

	//All get functions
	const std::vector<Body*>& GetBodysList() const;
	
	//All set functions
	void SetGravity(float gravity);

	void SetContactListener(WorldContactListener *listener);

	void SetSpaceDivisionQuadTree(SpaceDivisionQuadTree *sdQuadTree);

	//Update world (update all the objects in this world)
	void Update(float dt);

	void DestroyBody(Body* body);

	Body* CreateBody(const BodyDef &bodyDef);

	void AddBody(Body *body);

	void AddBody(const std::vector<Body*> &bodies);

	//Render all bodies by squares
	void RenderBodiesDebug(SpriteBatch *batch);

	void Release();

};

#endif