#include "WorldCollisionListener.h"

void WorldCollisionListener::OnCollisionEnter(Body * bodyA, Body * bodyB, const Vector2 & CollisionDirection)
{
	switch (bodyA->categoryBits * bodyB->categoryBits + (bodyA->categoryBits + bodyB->categoryBits))
	{
	case PLAYER_BIT*BULLET_BIT + PLAYER_BIT + BULLET_BIT:
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
	case BRICK_BIT*BULLET_BIT + BRICK_BIT + BULLET_BIT:
	{
		if (bodyA->categoryBits == BULLET_BIT || bodyB->categoryBits == BULLET_BIT) {
			ServerNetworkManager::Instance->DestroyNetworkGameObject(((NetworkGameObject*)(bodyA->GetExtra()))->GetNetworkId());
			ServerNetworkManager::Instance->DestroyNetworkGameObject(((NetworkGameObject*)(bodyB->GetExtra()))->GetNetworkId());
		}
		break;
	}
	case METAL_BIT*BULLET_BIT + METAL_BIT + BULLET_BIT: //84
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
	case BULLET_BIT*BOUND_BIT + BULLET_BIT + BOUND_BIT:
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
	case ENEMY_BIT*METAL_BIT + ENEMY_BIT + METAL_BIT:
	case ENEMY_BIT * BOUND_BIT + ENEMY_BIT + BOUND_BIT:
	{
		int rand = 0;
		if (bodyA->categoryBits == ENEMY_BIT) {
			Enemy* enemy = (Enemy*)(bodyA->GetExtra());
			Vector2 velocity = bodyA->GetVelocity();
			rand = std::rand() % 4 + 1;
			int dir = velocity.x < 0 ? 1 : velocity.x > 0 ? 3 : velocity.y > 0 ? 2 : 4;
			while (rand == dir)
			{
				rand = std::rand() % 4 + 1;
			}
			switch (rand)
			{
			case 1:
				bodyA->SetVelocity(-enemy->GetSpeed(), 0);
				break;
			case 2: 
				bodyA->SetVelocity(0, enemy->GetSpeed());
				break;
			case 3: 
				bodyA->SetVelocity(enemy->GetSpeed(), 0);
				break;
			case 4: 
				bodyA->SetVelocity(0, -enemy->GetSpeed());
				break;
			default:
				break;
			}
			
			ServerNetworkManager::Instance->UpdateNetworkGameObject(enemy->GetNetworkId(), enemy->ERS_Velocity);
		}
		else if (bodyB->categoryBits == ENEMY_BIT) {
			Enemy* enemy = (Enemy*)(bodyB->GetExtra());
			Vector2 velocity = bodyB->GetVelocity();
			rand = std::rand() % 4 + 1;
			int dir = velocity.x < 0 ? 1 : velocity.x > 0 ? 3 : velocity.y > 0 ? 2 : 4;
			while (rand == dir)
			{
				rand = std::rand() % 4 + 1;
			}
			switch (rand)
			{
			case 1:
				bodyB->SetVelocity(-enemy->GetSpeed(), 0);
				break;
			case 2:
				bodyB->SetVelocity(0, enemy->GetSpeed());
				break;
			case 3:
				bodyB->SetVelocity(enemy->GetSpeed(), 0);
				break;
			case 4:
				bodyB->SetVelocity(0, -enemy->GetSpeed());
				break;
			default:
				break;
			}

			ServerNetworkManager::Instance->UpdateNetworkGameObject(enemy->GetNetworkId(), enemy->ERS_Velocity);
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
	case BULLET_BIT*BOUND_BIT:
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
