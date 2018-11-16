#ifndef COLLISION_H
#define COLLISION_H

#include "Body.h"
#include <iostream>
#include <Windows.h>

#define NOT_COLLIDED 10000.0f

class Collision
{
private:

	// Kết quả của thuật toán kiểm tra va chạm,
	// không có va chạm sẽ trả về 1, 
	float _CollisionRatio;

	// Kết quả của va chạm tác động đến object,
	// dùng đề xác định hướng va chạm của các object (trên dưới trái phải)
	Vector2 _CollisionDirection;

	// Thời gian còn lại trong 1 frame sau khi va chạm xảy ra 
	// (thời gian của 1 frame > thời điểm va chạm - thời điểm bắt đầu frame)
	float _RemainingTime;

	Vector2 _CollisionPosition; // Vị trí của vật di chuyển khi va chạm với vật đứng yên

	float tempvx, tempvy; //velocity của vật a khi xem vật b đang đứng yên(nếu thực sự vật b đang di chuyển)
	float dxentry, dyentry, dxexit, dyexit; //dx entry / exit, dy entry / exit, khoảng d cần cần vật a đụng độ/ thoát ra khỏi vật b đứng yên (delta d của vật a trong khoảng delta_time)
	float deltadx, deltady; //khoảng delta vật a đi được trong khoảng thời gian delta_time
	float rxentry, rxexit, ryentry, ryexit; 
	float rentry;  //khoảng thời gian cần để thực sự xảy ra đụng độ
	float rexit; 	//khoảng thời gian cần để vật a thực sự thoát khỏi vật b

	float moveX, moveY;

	bool IsSensorEntered;

private: //private function
	
	void UpdateTargetPosition(Body *body,const Vector2 &move);

	void Push(Body *body);
	void Slide(Body *body);
	void Deflect(Body *body);

	friend class World;


public:
	Collision();
	~Collision();

	RECT GetBroadphaseRect(Body *body, float DeltaTime);
	RECT GetRECT(Body *body);
	// Hàm dùng để dự đoán vùng bao phủ của object trong frame kế
	bool IsOverlayingRect(const RECT &rect1, const RECT &rect2);

	void PerformCollision(Body *targetBody, Body *otherBody, float DeltaTime, int collisionAction, bool &needMoveX, bool &needMoveY);

	bool IsColliding(Body *targetBody, Body *otherBody, float DeltaTime);
	
	//bool IsPreviousCollding(Body *targetBody, Body *otherBody, float DeltaTime);

	//return 1 if bottom or top, return 2 if left or right and return 0 if not previous touching
	int IsPreviousTouching(Body *targetBody, Body *otherBody);
	
	int IsTouching(Body *targetBody, Body *otherBody);

	// 2 body có chồng lên nhau hay không
	bool IsOverlaying(Body *targetBody, Body *otherBody);

	bool IsPreviousOverlayed(Body *targetBody, Body *otherBody);


	void PerformOverlaying(Body *targetBody, Body *otherBody, bool &needMoveX, bool &needMoveY);
	
	//void SetContactListener(WorldContactListener *listener);

	//Reset to use as a new one
	void Reset();


};

#endif