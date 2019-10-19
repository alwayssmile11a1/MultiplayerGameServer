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
			Player* player = (Player*)(bodyB->GetExtra());

			if (bullet->GetPlayerNetworkGameObjectId() != player->GetNetworkId())
			{
				ServerNetworkManager::Instance->DestroyNetworkGameObject(((NetworkGameObject*)(bodyA->GetExtra()))->GetNetworkId());
				ServerNetworkManager::Instance->DestroyNetworkGameObject(((NetworkGameObject*)(bodyB->GetExtra()))->GetNetworkId());
			}
		}
		else
		{
			if (bodyB->categoryBits == BULLET_BIT)
			{
				Bullet* bullet = (Bullet*)(bodyB->GetExtra());
				Player* player = (Player*)(bodyA->GetExtra());

				if (bullet->GetPlayerNetworkGameObjectId() != player->GetNetworkId())
				{
					ServerNetworkManager::Instance->DestroyNetworkGameObject(((NetworkGameObject*)(bodyA->GetExtra()))->GetNetworkId());
					ServerNetworkManager::Instance->DestroyNetworkGameObject(((NetworkGameObject*)(bodyB->GetExtra()))->GetNetworkId());
				}
			}
		}
		break;
	}
	case BRICK_BIT*BULLET_BIT:
	{
		if (bodyA->categoryBits == BULLET_BIT || bodyB->categoryBits == BULLET_BIT) {
			ServerNetworkManager::Instance->DestroyNetworkGameObject(((NetworkGameObject*)(bodyA->GetExtra()))->GetNetworkId());
			ServerNetworkManager::Instance->DestroyNetworkGameObject(((NetworkGameObject*)(bodyB->GetExtra()))->GetNetworkId());
		}
		break;
	}
	case METAL_BIT*BULLET_BIT:
	{
		if (bodyA->categoryBits == BULLET_BIT)
		{
			ServerNetworkManager::Instance->DestroyNetworkGameObject(((NetworkGameObject*)(bodyA->GetExtra()))->GetNetworkId());
		}
		else
		{
			if (bodyB->categoryBits == BULLET_BIT)
			{
				ServerNetworkManager::Instance->DestroyNetworkGameObject(((NetworkGameObject*)(bodyB->GetExtra()))->GetNetworkId());
			}
		}
		break;
	}
	}
}

void WorldCollisionListener::OnColliding(Body * bodyA, Body * bodyB, const Vector2 & collisionDirection)
{
	switch (bodyA->categoryBits * bodyB->categoryBits)
	{
	case PLAYER_BIT*BULLET_BIT:
	{
		if (bodyA->categoryBits == BULLET_BIT)
		{
			Bullet* bullet = (Bullet*)(bodyA->GetExtra());
			Player* player = (Player*)(bodyB->GetExtra());

			if (bullet->GetPlayerNetworkGameObjectId() != player->GetNetworkId())
			{
				ServerNetworkManager::Instance->DestroyNetworkGameObject(((NetworkGameObject*)(bodyA->GetExtra()))->GetNetworkId());
				ServerNetworkManager::Instance->DestroyNetworkGameObject(((NetworkGameObject*)(bodyB->GetExtra()))->GetNetworkId());
			}
		}
		else
		{
			if (bodyB->categoryBits == BULLET_BIT)
			{
				Bullet* bullet = (Bullet*)(bodyB->GetExtra());
				Player* player = (Player*)(bodyA->GetExtra());

				if (bullet->GetPlayerNetworkGameObjectId() != player->GetNetworkId())
				{
					ServerNetworkManager::Instance->DestroyNetworkGameObject(((NetworkGameObject*)(bodyA->GetExtra()))->GetNetworkId());
					ServerNetworkManager::Instance->DestroyNetworkGameObject(((NetworkGameObject*)(bodyB->GetExtra()))->GetNetworkId());
				}
			}
		}
		break;
	}
	case BRICK_BIT*BULLET_BIT:
	{
		if (bodyA->categoryBits == BULLET_BIT || bodyB->categoryBits == BULLET_BIT) {
			ServerNetworkManager::Instance->DestroyNetworkGameObject(((NetworkGameObject*)(bodyA->GetExtra()))->GetNetworkId());
			ServerNetworkManager::Instance->DestroyNetworkGameObject(((NetworkGameObject*)(bodyB->GetExtra()))->GetNetworkId());
		}
		break;
	}
	case METAL_BIT*BULLET_BIT:
	{
		if (bodyA->categoryBits == BULLET_BIT)
		{
			ServerNetworkManager::Instance->DestroyNetworkGameObject(((NetworkGameObject*)(bodyA->GetExtra()))->GetNetworkId());
		}
		else
		{
			if (bodyB->categoryBits == BULLET_BIT)
			{
				ServerNetworkManager::Instance->DestroyNetworkGameObject(((NetworkGameObject*)(bodyB->GetExtra()))->GetNetworkId());
			}
		}
		break;
	}
	}
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
