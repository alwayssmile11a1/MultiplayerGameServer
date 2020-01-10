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

			if (bullet->GetIgnoredNetworkGameObjectId() != player->GetNetworkId())
			{
				ServerNetworkManager::Instance->DestroyNetworkGameObject(bullet->GetNetworkId());
				if (player->HasShield())
				{
					player->SetHasShield(false);
				}
				else
				{
					ServerNetworkManager::Instance->DestroyNetworkGameObject(player->GetNetworkId());
				}
			}
		}
		else
		{
			if (bodyB->categoryBits == BULLET_BIT)
			{
				Bullet* bullet = (Bullet*)(bodyB->GetExtra());
				Player* player = (Player*)(bodyA->GetExtra());

				if (bullet->GetIgnoredNetworkGameObjectId() != player->GetNetworkId())
				{
					ServerNetworkManager::Instance->DestroyNetworkGameObject(bullet->GetNetworkId());
					if (player->HasShield())
					{
						player->SetHasShield(false);
					}
					else
					{
						ServerNetworkManager::Instance->DestroyNetworkGameObject(player->GetNetworkId());
					}
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
	case ENEMY_BIT * METAL_BIT + ENEMY_BIT + METAL_BIT:
	case ENEMY_BIT * BOUND_BIT + ENEMY_BIT + BOUND_BIT:
	case ENEMY_BIT * BRICK_BIT + ENEMY_BIT + BRICK_BIT:
	{
		Enemy* enemy = nullptr;
		if (bodyA->categoryBits == ENEMY_BIT) {
			enemy = (Enemy*)(bodyA->GetExtra());
		}
		else if (bodyB->categoryBits == ENEMY_BIT) {
			enemy = (Enemy*)(bodyB->GetExtra());
		}
		if (enemy != nullptr) enemy->RandomVelocity();
		break;
	}
	case ENEMY_BIT * BULLET_BIT + ENEMY_BIT + BULLET_BIT:
	{
		if (bodyA->categoryBits == BULLET_BIT || bodyB->categoryBits == BULLET_BIT) {
			ServerNetworkManager::Instance->DestroyNetworkGameObject(((NetworkGameObject*)(bodyA->GetExtra()))->GetNetworkId());
			ServerNetworkManager::Instance->DestroyNetworkGameObject(((NetworkGameObject*)(bodyB->GetExtra()))->GetNetworkId());
		}
		break;
	}
	}
}

void WorldCollisionListener::OnColliding(Body * bodyA, Body * bodyB, const Vector2 & collisionDirection)
{
	switch (bodyA->categoryBits * bodyB->categoryBits + bodyA->categoryBits + bodyB->categoryBits)
	{
	case PLAYER_BIT * BULLET_BIT + PLAYER_BIT + BULLET_BIT:
	{
		if (bodyA->categoryBits == BULLET_BIT)
		{
			Bullet* bullet = (Bullet*)(bodyA->GetExtra());
			Player* player = (Player*)(bodyB->GetExtra());

			if (bullet->GetIgnoredNetworkGameObjectId() != player->GetNetworkId())
			{
				ServerNetworkManager::Instance->DestroyNetworkGameObject(bullet->GetNetworkId());
				if (player->HasShield())
				{
					player->SetHasShield(false);
				}
				else
				{
					ServerNetworkManager::Instance->DestroyNetworkGameObject(player->GetNetworkId());
				}
			}
		}
		else
		{
			if (bodyB->categoryBits == BULLET_BIT)
			{
				Bullet* bullet = (Bullet*)(bodyB->GetExtra());
				Player* player = (Player*)(bodyA->GetExtra());

				if (bullet->GetIgnoredNetworkGameObjectId() != player->GetNetworkId())
				{
					ServerNetworkManager::Instance->DestroyNetworkGameObject(bullet->GetNetworkId());
					if (player->HasShield())
					{
						player->SetHasShield(false);
					}
					else
					{
						ServerNetworkManager::Instance->DestroyNetworkGameObject(player->GetNetworkId());
					}
				}
			}
		}
		break;
	}
	case BRICK_BIT * BULLET_BIT + BRICK_BIT + BULLET_BIT:
	{
		if (bodyA->categoryBits == BULLET_BIT || bodyB->categoryBits == BULLET_BIT) {
			ServerNetworkManager::Instance->DestroyNetworkGameObject(((NetworkGameObject*)(bodyA->GetExtra()))->GetNetworkId());
			ServerNetworkManager::Instance->DestroyNetworkGameObject(((NetworkGameObject*)(bodyB->GetExtra()))->GetNetworkId());
		}
		break;
	}
	case METAL_BIT * BULLET_BIT + METAL_BIT + BULLET_BIT:
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
	case BULLET_BIT * BOUND_BIT + BULLET_BIT + BOUND_BIT:
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
	case ENEMY_BIT * METAL_BIT + ENEMY_BIT + METAL_BIT:
	case ENEMY_BIT * BOUND_BIT + ENEMY_BIT + BOUND_BIT:
	case ENEMY_BIT * BRICK_BIT + ENEMY_BIT + BRICK_BIT:
	{
		Enemy* enemy = nullptr;
		if (bodyA->categoryBits == ENEMY_BIT) {
			enemy = (Enemy*)(bodyA->GetExtra());
		}
		else if (bodyB->categoryBits == ENEMY_BIT) {
			enemy = (Enemy*)(bodyB->GetExtra());
		}
		if (enemy != nullptr) enemy->RandomVelocity();
		break;
	}
	case ENEMY_BIT * BULLET_BIT + ENEMY_BIT + BULLET_BIT:
	{
		if (bodyA->categoryBits == BULLET_BIT || bodyB->categoryBits == BULLET_BIT) {
			ServerNetworkManager::Instance->DestroyNetworkGameObject(((NetworkGameObject*)(bodyA->GetExtra()))->GetNetworkId());
			ServerNetworkManager::Instance->DestroyNetworkGameObject(((NetworkGameObject*)(bodyB->GetExtra()))->GetNetworkId());
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
	switch (bodyA->categoryBits * bodyB->categoryBits + bodyA->categoryBits + bodyB->categoryBits)
	{
	case PLAYER_BIT * STAR_BIT + PLAYER_BIT + STAR_BIT:
	{
		if (bodyA->categoryBits == STAR_BIT)
		{
			StarItem* starItem = (StarItem*)(bodyA->GetExtra());
			Player* player = (Player*)(bodyB->GetExtra());
			if (starItem != nullptr)
			{
				ServerNetworkManager::Instance->DestroyNetworkGameObject(starItem->GetNetworkId());
			}
			if (player != nullptr)
			{
				player->UpgradePlayer();
			}
		}
		else
		{
			if (bodyB->categoryBits == STAR_BIT)
			{
				StarItem* starItem = (StarItem*)(bodyB->GetExtra());
				Player* player = (Player*)(bodyA->GetExtra());
				if (starItem != nullptr)
				{
					ServerNetworkManager::Instance->DestroyNetworkGameObject(starItem->GetNetworkId());
				}
				if (player != nullptr)
				{
					player->UpgradePlayer();
				}
			}
		}
		break;
	}
	case PLAYER_BIT * SHIELD_BIT + PLAYER_BIT + SHIELD_BIT:
	{
		if (bodyA->categoryBits == SHIELD_BIT)
		{
			ShieldItem* shieldItem = (ShieldItem*)(bodyA->GetExtra());
			Player* player = (Player*)(bodyB->GetExtra());
			if (shieldItem != nullptr)
			{
				ServerNetworkManager::Instance->DestroyNetworkGameObject(shieldItem->GetNetworkId());
			}
			if (player != nullptr)
			{
				player->SetHasShield(true);
			}
		}
		else
		{
			if (bodyB->categoryBits == SHIELD_BIT)
			{
				ShieldItem* shieldItem = (ShieldItem*)(bodyB->GetExtra());
				Player* player = (Player*)(bodyA->GetExtra());
				if (shieldItem != nullptr)
				{
					ServerNetworkManager::Instance->DestroyNetworkGameObject(shieldItem->GetNetworkId());
				}
				if (player != nullptr)
				{
					player->SetHasShield(true);
				}
			}
		}
		break;
	}
	}
}

void WorldCollisionListener::OnSersorOverlaying(Body * bodyA, Body * bodyB)
{
	switch (bodyA->categoryBits * bodyB->categoryBits + bodyA->categoryBits + bodyB->categoryBits)
	{
	case PLAYER_BIT * STAR_BIT + PLAYER_BIT + STAR_BIT:
	{
		if (bodyA->categoryBits == STAR_BIT)
		{
			StarItem* starItem = (StarItem*)(bodyA->GetExtra());
			Player* player = (Player*)(bodyB->GetExtra());
			if (starItem != nullptr)
			{
				ServerNetworkManager::Instance->DestroyNetworkGameObject(starItem->GetNetworkId());
			}
			if (player != nullptr)
			{
				player->UpgradePlayer();
			}
		}
		else
		{
			if (bodyB->categoryBits == STAR_BIT)
			{
				StarItem* starItem = (StarItem*)(bodyB->GetExtra());
				Player* player = (Player*)(bodyA->GetExtra());
				if (starItem != nullptr)
				{
					ServerNetworkManager::Instance->DestroyNetworkGameObject(starItem->GetNetworkId());
				}
				if (player != nullptr)
				{
					player->UpgradePlayer();
				}
			}
		}
		break;
	}
	case PLAYER_BIT * SHIELD_BIT + PLAYER_BIT + SHIELD_BIT:
	{
		if (bodyA->categoryBits == SHIELD_BIT)
		{
			ShieldItem* shieldItem = (ShieldItem*)(bodyA->GetExtra());
			Player* player = (Player*)(bodyB->GetExtra());
			if (shieldItem != nullptr)
			{
				ServerNetworkManager::Instance->DestroyNetworkGameObject(shieldItem->GetNetworkId());
			}
			if (player != nullptr)
			{
				player->SetHasShield(true);
			}
		}
		else
		{
			if (bodyB->categoryBits == SHIELD_BIT)
			{
				ShieldItem* shieldItem = (ShieldItem*)(bodyB->GetExtra());
				Player* player = (Player*)(bodyA->GetExtra());
				if (shieldItem != nullptr)
				{
					ServerNetworkManager::Instance->DestroyNetworkGameObject(shieldItem->GetNetworkId());
				}
				if (player != nullptr)
				{
					player->SetHasShield(true);
				}
			}
		}
		break;
	}
	}
}

void WorldCollisionListener::OnSensorExit(Body * bodyA, Body * bodyB)
{
}
