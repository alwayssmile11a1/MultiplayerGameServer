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
#include <unordered_map>

#define USECOLLISIONINFO 0 //using collision info is more precise for collision callback, but decreasing performance


// add body to this class and physic things will be handled
class World: public GameObject
{
private:
	
	float _Gravity;  
	std::vector<Body*> _Bodies; 	
									

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

	//Check and simulate collision only
	void UpdateForBody(Body* body, float dt);

	void DestroyBody(Body* body);

	Body* CreateBody(const BodyDef &bodyDef);

	void AddBody(Body *body);

	void AddBody(const std::vector<Body*> &bodies);

	//Render all bodies by squares
	void RenderBodiesDebug(SpriteBatch *batch);

	//void Release();

};

typedef std::shared_ptr<World> WorldPtr;

//just a holder for worlds, not automatically release anything
class WorldCollector
{
private:
	static std::unordered_map<int, WorldPtr> mNameToWorldMap;
public:
	static WorldPtr CreateWorld(int id);
	static WorldPtr GetWorld(int id);
	static void RemoveWorld(int id);

};

#endif