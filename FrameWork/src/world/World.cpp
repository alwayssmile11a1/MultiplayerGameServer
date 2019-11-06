#include"World.h"
#include "stdafx.h"

World::World()
{
	_Gravity = 9.8f;
	_Bodies.clear();
	_Cam = NULL;
	if (_QuadTree != NULL)
	{
		_QuadTree->Clear();
		delete _QuadTree;
	}
	_QuadTree = new QuadTree();


	_SDQuadTree = NULL;

}

World::World(float gravity)
{
	_Gravity = gravity;
	_Bodies.clear();
	_Cam = NULL;
	if (_QuadTree != NULL)
	{
		_QuadTree->Clear();
		delete _QuadTree;
	}
	_QuadTree = new QuadTree();
}

World::~World()
{
	if (_QuadTree != NULL)
	{
		_QuadTree->Clear();
		delete _QuadTree;
	}

	for (std::vector<Body*>::iterator obj = _Bodies.begin(); obj != _Bodies.end(); ++obj)
	{
		//just for sure
		if (*obj != NULL)
		{
			delete *obj;
			(*obj) = NULL;
		}
	}
}

//void World::Release()
//{
//	if (_QuadTree != NULL)
//	{
//		_QuadTree->Clear();
//		delete _QuadTree;
//	}
//
//	for (std::vector<Body*>::iterator obj = _Bodies.begin(); obj != _Bodies.end(); ++obj)
//	{
//		//just for sure
//		if (*obj != NULL)
//		{
//			delete *obj;
//			(*obj) = NULL;
//		}
//	}
//
//	//for (std::vector<Body*>::iterator obj = _SDQBodies.begin(); obj != _SDQBodies.end(); ++obj)
//	//{
//	//	//just for sure
//	//	if (*obj != NULL)
//	//	{
//	//		delete *obj;
//	//		(*obj) = NULL;
//	//	}
//	//}
//
//
//
//}

//World::World(const World &world)
//{
//	this->_Gravity = world._Gravity;
//	/*std::vector<Object*> vectorCopy = world.GetObjectsList();
//	for (std::vector<Object*>::iterator obj = vectorCopy.begin(); obj != vectorCopy.end(); ++obj)
//	{
//		Object * temp = new Object();
//		*temp = **obj;
//		this->Objects.push_back(temp);
//	}*/
//}
//
//World& World::operator=(const World &world)
//{
//	this->_Gravity = world._Gravity;
//	//std::vector<Object*> vectorCopy = world.GetObjectsList();
//	//for (std::vector<Object*>::iterator obj = vectorCopy.begin(); obj != vectorCopy.end(); ++obj)
//	//{
//	//	Object * temp = new Object();
//	//	*temp = **obj;
//	//	this->Objects.push_back(temp);
//	//}
//	return *this;
//}

const std::vector<Body*>& World::GetBodysList() const
{
	return _Bodies;
}

void World::SetGravity(float gravity)
{
	_Gravity = gravity;
}

void World::UpdateForBody(Body* body, float dt)
{

	if (body->_BodyType == Body::BodyType::Static)
	{
		body->Next(dt, false, false);
		return;
	}

	//calculate actual velocity
	body->CalculateActualVelocity(dt, _Gravity);

	//get broadphase rect 
	RECT broadphaseRect = collision.GetBroadphaseRect(body, dt);

	bool moveX = true, moveY = true;

	for (int j = 0; j < _Bodies.size(); j++)
	{
		Body* body2 = _Bodies.at(j);
		if (body == body2 || body2->_IsSensor == true) continue;

		bool collide = ((body)->maskBits & (body2)->categoryBits) != 0 && ((body)->categoryBits & (body2)->maskBits) != 0;

		if (!collide) continue;

		//get static rect 
		RECT staticRect = collision.GetRECT(body2);

		//check collision
		CheckCollision(body, body2, broadphaseRect, staticRect, moveX, moveY, dt);

		collision.Reset();
	}

	body->Next(dt, moveX, moveY);
}

void World::Update(float dt)
{

	if (_SDQuadTree != NULL)
	{

		//a vector of all bodies could collide each other 
		std::vector<Body*> bodiesCouldCollide;

		bodiesCouldCollide.insert(bodiesCouldCollide.end(), _SDQuadTree->bodiesInViewport.begin(), _SDQuadTree->bodiesInViewport.end());
		bodiesCouldCollide.insert(bodiesCouldCollide.end(), _Bodies.begin(), _Bodies.end());


		for (int i = 0; i < bodiesCouldCollide.size(); i++)
		{
			Body* body1 = bodiesCouldCollide.at(i);

			if (body1->_BodyType == Body::BodyType::Static)
			{
				(body1)->Next(dt, false, false);
				continue;
			}

			//calculate actual velocity
			body1->CalculateActualVelocity(dt, _Gravity);

			//get broadphase rect 
			RECT broadphaseRect = collision.GetBroadphaseRect(body1, dt);

			bool moveX = true, moveY = true;

			for (int j = 0; j < bodiesCouldCollide.size(); j++)
			{
				Body* body2 = bodiesCouldCollide.at(j);
				if (body1 == body2 || body2->_IsSensor == true) continue;

				bool collide = ((body1)->maskBits & (body2)->categoryBits) != 0 && ((body1)->categoryBits & (body2)->maskBits) != 0;

				if (!collide) continue;

				//get static rect 
				RECT staticRect = collision.GetRECT(body2);

				//check collision
				CheckCollision(body1, body2, broadphaseRect, staticRect, moveX, moveY, dt);

				collision.Reset();
			}

			body1->Next(dt, moveX, moveY);

		}

	}
	else
	{

		if (_Cam == NULL)
		{
			for (int i = 0; i < _Bodies.size(); i++)
			{
				Body* body1 = _Bodies.at(i);

				if (body1->_BodyType == Body::BodyType::Static)
				{
					(body1)->Next(dt, false, false);
					continue;
				}

				//calculate actual velocity
				body1->CalculateActualVelocity(dt, _Gravity);

				//get broadphase rect 
				RECT broadphaseRect = collision.GetBroadphaseRect(body1, dt);

				bool moveX = true, moveY = true;

				for (int j = 0; j < _Bodies.size(); j++)
				{
					Body* body2 = _Bodies.at(j);
					if (body1 == body2 || body2->_IsSensor == true) continue;

					bool collide = ((body1)->maskBits & (body2)->categoryBits) != 0 && ((body1)->categoryBits & (body2)->maskBits) != 0;

					if (!collide) continue;

					//get static rect 
					RECT staticRect = collision.GetRECT(body2);

					//check collision
					CheckCollision(body1, body2, broadphaseRect, staticRect, moveX, moveY, dt);

					collision.Reset();
				}

				body1->Next(dt, moveX, moveY);

			}
		}
		else
		{

			//_QuadTree = new QuadTree(0, Vector2((float)screenWidth, (float)screenHeight), );

			_QuadTree->Clear();

			_QuadTree->Update(Vector2((float)screenWidth, (float)screenHeight), _Cam->GetPosition());

			for (std::vector<Body*>::iterator body = _Bodies.begin(); body != _Bodies.end(); ++body)
			{
				Body* it = *body;

				Vector2 campos = Vector2(_Cam->GetPosition().x - (int)screenWidth / 2, _Cam->GetPosition().y - (int)screenHeight / 2);

				/*if (((it->GetPosition().x + it->GetSize().x > campos.x) && (it->GetPosition().x < campos.x + screenWidth)) &&
				((it->GetPosition().y + it->GetSize().y > campos.y) && (it->GetPosition().y < campos.y + screenHeight)))
				_QuadTree->Insert(*body);*/

				//Vector2 campos = Vector2(_Cam->GetPosition().x - screenWidth / 2, _Cam->GetPosition().y - screenHeight / 2);

				/*if (((it->GetPosition().x >= campos.x && it->GetPosition().x <= campos.x + screenWidth) || (it->GetPosition().y >= campos.y && it->GetPosition().y <= campos.y + screenHeight))
				&&
				(!(it->GetPosition().x > campos.x + screenWidth || it->GetPosition().x + it->GetSize().x < campos.x))
				&&
				(!(it->GetPosition().y > campos.y + screenHeight || it->GetPosition().y + it->GetSize().y < campos.y)))
				{
				_QuadTree->Insert(*body);
				}*/

				bool botleft = it->GetPosition().x >= campos.x && it->GetPosition().x <= campos.x + screenWidth && it->GetPosition().y >= campos.y && it->GetPosition().y <= campos.y + screenHeight;
				bool topleft = it->GetPosition().x >= campos.x && it->GetPosition().x <= campos.x + screenWidth && it->GetPosition().y + it->GetSize().y >= campos.y && it->GetPosition().y + it->GetSize().y <= campos.y + screenHeight;
				bool botright = it->GetPosition().x + it->GetSize().x >= campos.x && it->GetPosition().x + it->GetSize().x <= campos.x + screenWidth && it->GetPosition().y >= campos.y && it->GetPosition().y <= campos.y + screenHeight;
				bool topright = it->GetPosition().x + it->GetSize().x >= campos.x && it->GetPosition().x + it->GetSize().x <= campos.x + screenWidth && it->GetPosition().y + it->GetSize().y >= campos.y && it->GetPosition().y + it->GetSize().y <= campos.y + screenHeight;

				if (botleft || topleft || botright || topright)
					_QuadTree->Insert(*body);
				else
				{
					bool checkx = it->GetPosition().x + it->GetSize().x > campos.x && it->GetPosition().x < campos.x + screenWidth;
					bool checky = it->GetPosition().y + it->GetSize().y > campos.y && it->GetPosition().y < campos.y + screenHeight;
					if (checkx && checky)
						_QuadTree->Insert(*body);
				}

			}

			std::vector<Body*> _BodiesCouldCollide;

			for (std::vector<Body*>::iterator it1 = _Bodies.begin(); it1 != _Bodies.end(); ++it1)
			{
				Body* body1 = *it1;
				if (body1->_BodyType == Body::BodyType::Static)
				{
					body1->Next(dt, false, false);
					continue;
				}

				body1->CalculateActualVelocity(dt, _Gravity);

				RECT broadphaseRect = collision.GetBroadphaseRect(body1, dt);

				bool moveX = true, moveY = true;

				_BodiesCouldCollide.clear();

				_QuadTree->Retrieve(_BodiesCouldCollide, body1);

				for (std::vector<Body*>::iterator it2 = _BodiesCouldCollide.begin(); it2 != _BodiesCouldCollide.end(); ++it2)
				{
					Body* body2 = *it2;
					if (body1 == (body2) || body2->_IsSensor == true) continue;

					bool collide = (body1->maskBits & body2->categoryBits) != 0 && (body1->categoryBits & body2->maskBits) != 0;

					if (!collide) continue;

					//get static rect 
					RECT staticRect = collision.GetRECT(body2);

					//check collision
					CheckCollision(body1, body2, broadphaseRect, staticRect, moveX, moveY, dt);


					collision.Reset();
				}

				//Stop objects outside of window from updating
				Vector2 campos = Vector2(_Cam->GetPosition().x - (int)screenWidth / 2, _Cam->GetPosition().y - (int)screenHeight / 2);
				if (((body1->GetPosition().x + body1->GetSize().x > campos.x) && (body1->GetPosition().x < campos.x + screenWidth)) &&
					((body1->GetPosition().y + body1->GetSize().y > campos.y) && (body1->GetPosition().y < campos.y + screenHeight)))
					body1->Next(dt, moveX, moveY);
			}
		}

	}

}

void World::CheckCollision(Body* body1, Body* body2, const RECT &broadphaseRect, const RECT &staticRect, bool &moveX, bool &moveY, float dt)
{
#if USECOLLISIONINFO

	CollisionInfo* collisionInfo = body1->_CollisionPairStates.at(body2);

	if (!body1->_IsSensor) //a normal body
	{
		//check overlaying broadphase
		if (collision.IsOverlayingRect(broadphaseRect, staticRect))
		{
			//check colliding
			if (collision.IsColliding(body1, body2, dt))
			{

				/*if (collision.rxentry != 0 && collision.ryentry != 0)*/
				if (collisionInfo->collisionState == CollisionInfo::CollisionType::Exit || collisionInfo->collisionState == CollisionInfo::CollisionType::No)
				{
					_Listener->OnCollisionEnter(body1, body2, collision._CollisionDirection);
					collisionInfo->collisionState = CollisionInfo::CollisionType::Enter;
				}
				else
				{
					_Listener->OnColliding(body1, body2, collision._CollisionDirection);
					collisionInfo->collisionState = CollisionInfo::CollisionType::On;
				}


				collision.PerformCollision(body1, body2, dt, 0, moveX, moveY);

			}
			else //if not, check collision exit callback
			{
				//int touching = collision.IsTouching(body1, body2); //If istouching, it means in the next frame, two body will not collide anymore
				//if (touching == 1 && body1->GetVelocity().y != 0)
				//{
				//	_Listener->OnCollisionExit(body1, body2, collision._CollisionDirection);
				//}
				//else
				//{

				//	if (touching == 2 && body1->GetVelocity().x != 0)
				//	{
				//		_Listener->OnCollisionExit(body1, body2, collision._CollisionDirection);
				//	}
				//}


				if (collisionInfo->collisionState == CollisionInfo::CollisionType::On)
				{
					_Listener->OnCollisionExit(body1, body2, collision._CollisionDirection);
					collisionInfo->collisionState = CollisionInfo::CollisionType::Exit;
				}

			}

			//if (!body2->_BodyType == Body::BodyType::Static) //check overlaying because sometimes, 
			//												//two dynamic object still overlaying each other after performing colliding 
			//{
			if (collision.IsOverlaying(body1, body2))
			{
				collision.PerformOverlaying(body1, body2, moveX, moveY);
			}
			/*}*/

		}
	}
	else //is sensor
	{

		if (collision.IsOverlaying(body1, body2)) //check sensor overlaying
		{
			//if (!collision.IsPreviousOverlayed(body1, body2)) //if it's previous overlayed, it means the sensor has just entered 
			//{
			//	_Listener->OnSersorEnter(body1, body2);

			//}
			//else //else, is overlaying
			//{
			//	_Listener->OnSersorOverlaying(body1, body2);
			//}

			if (collisionInfo->sensorState == CollisionInfo::CollisionType::Exit
				|| collisionInfo->sensorState == CollisionInfo::CollisionType::No)
			{
				_Listener->OnSersorEnter(body1, body2);
				collisionInfo->sensorState = CollisionInfo::CollisionType::Enter;
			}
			else
			{
				_Listener->OnSersorOverlaying(body1, body2);
				collisionInfo->sensorState = CollisionInfo::CollisionType::On;
			}



		}
		else //not overlaying
		{
			//if (collision.IsPreviousOverlayed(body1, body2)) //if it's previous overlayed, it means the sensor has just exit 
			//{
			//	_Listener->OnSensorExit(body1, body2);
			//}

			if (collisionInfo->sensorState == CollisionInfo::CollisionType::On)
			{
				_Listener->OnSensorExit(body1, body2);
				collisionInfo->sensorState = CollisionInfo::CollisionType::Exit;
			}

		}


	}


#else
	
if (!body1->_IsSensor) //If body1 is a normal body
{
	//check overlaying broadphase
	if (collision.IsOverlayingRect(broadphaseRect, staticRect))
	{
		if (collision.IsColliding(body1, body2, dt))
		{
			//check oncollisionenter callback
			if (collision.rxentry != 0 && collision.ryentry != 0)
			{
				_Listener->OnCollisionEnter(body1, body2, collision._CollisionDirection);
			}
			else //on colliding callback
			{
				_Listener->OnColliding(body1, body2, collision._CollisionDirection);
			}

			//perform collision
			collision.PerformCollision(body1, body2, dt, 0, moveX, moveY);

		}
		else //if not, check collision exit callback
		{
			int touching = collision.IsTouching(body1, body2); //If istouching, it means in the next frame, two body will not collide anymore
			if (touching == 1 && body1->GetVelocity().y != 0)
			{
				_Listener->OnCollisionExit(body1, body2, collision._CollisionDirection);
			}
			else
			{
				if (touching == 2 && body1->GetVelocity().x != 0)
				{
					_Listener->OnCollisionExit(body1, body2, collision._CollisionDirection);
				}
			}


		}

		//check overlaying (sometimes two bodies are already overlaying each other 
		if (collision.IsOverlaying(body1, body2))
		{
			collision.PerformOverlaying(body1, body2, moveX, moveY);
		}

	}

}
else //is sensor
{
	if (collision.IsOverlaying(body1, body2)) //check sensor overlaying
	{
		if (!collision.IsPreviousOverlayed(body1, body2)) //if it's previous overlayed, it means the sensor has just entered 
		{
			_Listener->OnSersorEnter(body1, body2);

		}
		else //else, is overlaying
		{
			_Listener->OnSersorOverlaying(body1, body2);
		}


	}
	else //not overlaying
	{
		if (collision.IsPreviousOverlayed(body1, body2)) //if it's previous overlayed, it means the sensor has just exit 
		{
			_Listener->OnSensorExit(body1, body2);
		}

	}


}


#endif


}

void World::AddBody(Body *body)
{
	_Bodies.push_back(body);


}

void World::AddBody(const std::vector<Body*> &bodies)
{
	_Bodies.insert(_Bodies.end(), bodies.begin(),bodies.end());



}

void  World::SetCamera(Camera *cam)
{
	_Cam = cam;
}

void World::SetContactListener(WorldContactListener *listener)
{
	_Listener = listener;
}

Body* World::CreateBody(const BodyDef &bodyDef)
{
	Body* body = new Body();
	body->_Position = bodyDef.position;
	body->_Size = bodyDef.size;
	body->_Mass = bodyDef.mass;
	body->_BodyType = bodyDef.bodyType;
	body->_IsSensor = bodyDef.isSensor;
	body->_LinearDrag = bodyDef.linearDrag;

	_Bodies.push_back(body);
	
#if USECOLLISIONINFO

	//pair collision info
	for (std::vector<Body*>::iterator it = _Bodies.begin(); it != _Bodies.end(); ++it)
	{
		(*it)->_CollisionPairStates[body] = new CollisionInfo();
		body->_CollisionPairStates[*it] = new CollisionInfo();
	}

#endif // USECOLLISIONINFO

	

	return body;
}


void World::DestroyBody(Body* body)
{
	if (body == NULL) return;

	//find in Bodies
	std::vector<Body*>::iterator it = std::find(_Bodies.begin(), _Bodies.end(), body);
	if (it != _Bodies.end())
	{

#if USECOLLISIONINFO
		//delete pair collision info
		for (std::vector<Body*>::iterator it = _Bodies.begin(); it != _Bodies.end(); ++it)
		{
			(*it)->_CollisionPairStates.erase(body);
		}
#endif 

		delete *it;
		_Bodies.erase(it);
		body = NULL;
	}

	if (_SDQuadTree != NULL && body!=NULL)
	{
		// Attempt to find and return a body using provided name, else return nullptr
		std::map<unsigned int, Body*>::iterator it = _SDQuadTree->mapBody.find(body->id);

		if (it != _SDQuadTree->GetMapBody().end())
		{
			delete it->second;
			it->second = NULL;
			body = NULL;
			_SDQuadTree->mapBody.erase(it);
		}
	}
}

void World::RenderBodiesDebug(SpriteBatch *batch)
{
	std::vector<Body*> allbodies;

	allbodies.insert(allbodies.end(), _Bodies.begin(), _Bodies.end());

	if (_SDQuadTree != NULL)
	{
		allbodies.insert(allbodies.end(), _SDQuadTree->GetBodiesInViewport().begin(), _SDQuadTree->GetBodiesInViewport().end());
	}

	for (std::vector<Body*>::iterator bo = allbodies.begin(); bo != allbodies.end(); ++bo)
	{
		float x = (*bo)->GetPosition().x;
		float y = (*bo)->GetPosition().y;
		float width = (*bo)->GetSize().x;
		float height = (*bo)->GetSize().y;

		//batch.Draw(_DebugSquareTexture, x, y, 0, 0, _DebugSquareTexture.GetImageSize().x, _DebugSquareTexture.GetImageSize().y, width, height);
		batch->DrawSquare(x, y, width, height, D3DCOLOR_ARGB(255, 0, 128, 0));
	}
}

void World::SetSpaceDivisionQuadTree(SpaceDivisionQuadTree *sdQuadTree)
{
	this->_SDQuadTree = sdQuadTree;
}


std::unordered_map<int, WorldPtr> WorldCollector::mNameToWorldMap;
WorldPtr WorldCollector::CreateWorld(int id)
{
	WorldPtr world = std::shared_ptr<World>(new World());
	mNameToWorldMap[id] = world;
	return world;
}

WorldPtr WorldCollector::GetWorld(int id)
{
	auto it = mNameToWorldMap.find(id);
	if (it != mNameToWorldMap.end())
	{
		return it->second;
	}
	else
	{
		return nullptr;
	}
}

void WorldCollector::RemoveWorld(int id)
{
	mNameToWorldMap.erase(id);
}