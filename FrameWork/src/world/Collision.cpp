#include "Collision.h"

//#include"..\console\Console.h"
//#include"..\others\Global.h"
Collision::Collision()
{
}


Collision::~Collision()
{
}

bool Collision::IsColliding(Body *targetBody, Body *otherBody, float DeltaTime)
{
	//if (!IsOverlayingBroadphaseRect(targetBody, otherBody, DeltaTime))
	//{
	//	_CollisionDirection.x = 0.0f;
	//	_CollisionDirection.y = 0.0f;
	//	_CollisionRatio = 1.0f;
	//	_RemainingTime = std::numeric_limits<float>::infinity();
	//	_CollisionPosition.x = 0.0f;
	//	_CollisionPosition.y = 0.0f;
	//	return false;
	//}

	Vector2 targetVelocity = targetBody->GetTotalVelocity();
	Vector2 otherVelocity = otherBody->GetTotalVelocity();

	//tính toán dx entry và dx exit, có 2 trường hợp là vật a di chuyển ngược và xuôi với trục toạ độ
	tempvx = targetVelocity.x - otherVelocity.x;
	tempvy = targetVelocity.y - otherVelocity.y;

	if (tempvx > 0.0f)
	{
		dxentry = (otherBody->GetPosition().x - otherBody->GetSize().x / 2) - (targetBody->GetPosition().x + targetBody->GetSize().x / 2);
		dxexit = (otherBody->GetPosition().x + otherBody->GetSize().x / 2) - (targetBody->GetPosition().x - targetBody->GetSize().x / 2);
	}
	else
	{
		dxentry = (otherBody->GetPosition().x + otherBody->GetSize().x / 2) - (targetBody->GetPosition().x - targetBody->GetSize().x / 2);
		dxexit = (otherBody->GetPosition().x - otherBody->GetSize().x / 2) - (targetBody->GetPosition().x + targetBody->GetSize().x / 2);
	}

	//tính toán dy entry và exit, tương tự với dx entry/ exit
	if (tempvy > 0.0f)
	{
		dyentry = (otherBody->GetPosition().y - otherBody->GetSize().y / 2) - (targetBody->GetPosition().y + targetBody->GetSize().y / 2);
		dyexit = (otherBody->GetPosition().y + otherBody->GetSize().y / 2) - (targetBody->GetPosition().y - targetBody->GetSize().y / 2);
	}
	else
	{
		dyentry = (otherBody->GetPosition().y + otherBody->GetSize().y / 2) - (targetBody->GetPosition().y - targetBody->GetSize().y / 2);
		dyexit = (otherBody->GetPosition().y - otherBody->GetSize().y / 2) - (targetBody->GetPosition().y + targetBody->GetSize().y / 2);
	}

	//tính toán t x entry/ exit
	if (tempvx == 0.0f) //tránh trường hợp a.velocity = 0 dẫn tới việc chia cho 0, nên ta gán x entry/ exit = +/-vô cùng
	{
		rxentry = -std::numeric_limits<float>::infinity();
		rxexit = std::numeric_limits<float>::infinity();
	}
	else
	{
		rxentry = dxentry / (tempvx*DeltaTime);
		rxexit = dxexit / (tempvx*DeltaTime);
	}

	//tính toán t y entry/ exit, tương tự x entry/ exit
	if (tempvy == 0.0f)
	{
		ryentry = -std::numeric_limits<float>::infinity();
		ryexit = std::numeric_limits<float>::infinity();
	}
	else
	{
		ryentry = dyentry / (tempvy*DeltaTime);
		ryexit = dyexit / (tempvy*DeltaTime);
	}

	// tính toán thời gian va chạm và thoát khỏi thực sự của vật a chuyển động đối với vật b đứng yên
	rentry = max(rxentry, ryentry);
	rexit = min(rxexit, ryexit);

	//sau khi tính toán được thời gian thực sự va chạm và thoát khỏi, ta kiểm tra xem việc va chạm có xảy ra hay không
	if ((rentry > rexit) || //trường hợp không xảy ra va chạm 1: thời gian thực sự xảy ra va chạm > thời gian thực sự a thoát khỏi b

		(rxentry < 0.0f && ryentry < 0.0f) || //trường hợp không xảy ra va chạm thứ 2: vật a có vận tốc = 0 dẫn đến x entry/ y entry = -vô cùng, hoặc vật a di chuyển hướng ra khỏi vật b

		(rxentry > 1.0f) || (ryentry > 1.0f)) //trường hợp không xảy ra va chạm thứ 3: trong khoảng thời gian delta_time đang xét (thời gian của 1 frame) thì vật a di chuyển chưa tới vật b

	{
		_CollisionDirection.x = NOT_COLLIDED;
		_CollisionDirection.y = NOT_COLLIDED;

		//int touching = IsTouching(targetBody, otherBody);
		//if (touching == 1 && targetVelocity.y != 0)
		//{
		//	_Listener->OnCollisionExit(targetBody, otherBody, _CollisionDirection);
		//}
		//else
		//{

		//	if (touching == 2 && targetVelocity.x != 0)
		//	{
		//		_Listener->OnCollisionExit(targetBody, otherBody, _CollisionDirection);
		//	}
		//}
	
		//if (IsPreviousCollding(targetBody, otherBody,DeltaTime))
		//{
		//	_Listener->OnCollisionExit(targetBody, otherBody, _CollisionDirection);
		//}

		_CollisionRatio = 1.0f;
		_RemainingTime = std::numeric_limits<float>::infinity();
		_CollisionPosition.x = 0.0f;
		_CollisionPosition.y = 0.0f;
		return false;
	}
	else //xảy ra va chạm, chia làm 4 trường hợp, tương ứng với 4 hướng va chạm của vật a so với vật b
	{
		if (rxentry > ryentry)//nếu việc xảy ra va chạm nằm trên trục x 
		{
			// nếu vật a đang nằm ngay sát vật b thì trả về là đang va chạm
			// và ngăn không cho vật a thay đổi theo x
			if (dxentry == 0.0f)//nếu vật a ngay sát bên phải/trái vật b thì không cho xảy ra va chạm
			{
				if (dyentry == 0.0f || dyexit == 0.0f)
				{
					_CollisionDirection.x = NOT_COLLIDED;
				}
				else
				{
					_CollisionDirection.x = -targetVelocity.x;
				}

				_CollisionDirection.y = NOT_COLLIDED; //hardcode 100.0f để báo là ko va chạm theo chiều này
			}
			else
			{
				if (dxentry < 0.0f)//nếu vật a nằm bên phải vật b => vật a va chạm cạnh bên phải của hình bao vật b
				{
					_CollisionDirection.x = abs(targetVelocity.x);
					_CollisionDirection.y = NOT_COLLIDED; //hardcode 100.0f để báo là ko va chạm theo chiều này
				}
				else //nếu vật a nằm bên trái vật b => vật a va chạm cạnh bên trái của hình bao vật b
				{
					_CollisionDirection.x = -abs(targetVelocity.x);
					_CollisionDirection.y = NOT_COLLIDED; //hardcode 100.0f để báo là ko va chạm theo chiều này
				}
			}
		}
		else//nếu việc xảy ra va chạm nằm trên trục y
		{
			// nếu vật a đang nằm ngay sát vật b thì trả về là đang va chạm
			// và ngăn không cho vật a thay đổi theo y
			if (dyentry == 0.0f)
			{
				_CollisionDirection.x = NOT_COLLIDED; //hardcode 100.0f để báo là ko va chạm theo chiều này
				if (dxentry == 0.0f || dxexit == 0.0f)
				{
					_CollisionDirection.y = NOT_COLLIDED;
				}
				else
				{
					_CollisionDirection.y = -targetVelocity.y;
				}
			}
			else
			{
				if (dyentry < 0.0f)//nếu vật a nằm bên dưới vật b => vật a sẽ va chạm cạnh dưới hình bao vât b
				{
					_CollisionDirection.x = NOT_COLLIDED; //hardcode 100.0f để báo là ko va chạm theo chiều này
					_CollisionDirection.y = abs(targetVelocity.y);
				}
				else//nếu vật a nằm bên trên vật b => vật a sẽ va chạm cạnh trên hình bao vât b
				{
					_CollisionDirection.x = NOT_COLLIDED; //hardcode 100.0f để báo là ko va chạm theo chiều này
					_CollisionDirection.y = -abs(targetVelocity.y);
				}
			}
		}

		//if (!targetBody->_IsSensor)
		//{
		//	if (rxentry != 0 && ryentry != 0)
		//	{
		//		_Listener->OnCollisionEnter(targetBody, otherBody, _CollisionDirection);
		//	}
		//	else
		//	{
		//		_Listener->OnColliding(targetBody, otherBody, _CollisionDirection);
		//	}
		//}
		//else
		//{
		//	_Listener->OnSersorEnter(targetBody, otherBody);
		//	IsSensorEntered = true;
		//}



		_CollisionRatio = rentry;
		_RemainingTime = DeltaTime - rentry*DeltaTime;

		_CollisionPosition.x = targetBody->GetPosition().x + rentry*DeltaTime*targetVelocity.x;
		_CollisionPosition.y = targetBody->GetPosition().y + rentry*DeltaTime*targetVelocity.y;


		return true;
	}
}

//bool Collision::IsPreviousCollding(Body *targetBody, Body *otherBody, float DeltaTime)
//{
//	Vector2 targetVelocity = Vector2(targetBody->_PreviousVelocity.x*100, targetBody->_PreviousVelocity.y*100);
//	Vector2 otherVelocity = Vector2(otherBody->_PreviousVelocity.x*100, otherBody->_PreviousVelocity.y*100);
//
//	//tính toán dx entry và dx exit, có 2 trường hợp là vật a di chuyển ngược và xuôi với trục toạ độ
//	tempvx = targetVelocity.x - otherVelocity.x;
//	tempvy = targetVelocity.y - otherVelocity.y;
//
//	if (tempvx > 0.0f)
//	{
//		dxentry = (otherBody->_PreviousPosition.x - otherBody->GetSize().x / 2) - (targetBody->_PreviousPosition.x + targetBody->GetSize().x / 2);
//		dxexit = (otherBody->_PreviousPosition.x + otherBody->GetSize().x / 2) - (targetBody->_PreviousPosition.x - targetBody->GetSize().x / 2);
//	}
//	else
//	{
//		dxentry = (otherBody->_PreviousPosition.x + otherBody->GetSize().x / 2) - (targetBody->_PreviousPosition.x - targetBody->GetSize().x / 2);
//		dxexit = (otherBody->_PreviousPosition.x - otherBody->GetSize().x / 2) - (targetBody->_PreviousPosition.x + targetBody->GetSize().x / 2);
//	}
//
//	//tính toán dy entry và exit, tương tự với dx entry/ exit
//	if (tempvy > 0.0f)
//	{
//		dyentry = (otherBody->_PreviousPosition.y - otherBody->GetSize().y / 2) - (targetBody->_PreviousPosition.y + targetBody->GetSize().y / 2);
//		dyexit = (otherBody->_PreviousPosition.y + otherBody->GetSize().y / 2) - (targetBody->_PreviousPosition.y - targetBody->GetSize().y / 2);
//	}
//	else
//	{
//		dyentry = (otherBody->_PreviousPosition.y + otherBody->GetSize().y / 2) - (targetBody->_PreviousPosition.y - targetBody->GetSize().y / 2);
//		dyexit = (otherBody->_PreviousPosition.y - otherBody->GetSize().y / 2) - (targetBody->_PreviousPosition.y + targetBody->GetSize().y / 2);
//	}
//
//	//tính toán t x entry/ exit
//	if (targetVelocity.x == 0.0f) //tránh trường hợp a.velocity = 0 dẫn tới việc chia cho 0, nên ta gán x entry/ exit = +/-vô cùng
//	{
//		rxentry = -std::numeric_limits<float>::infinity();
//		rxexit = std::numeric_limits<float>::infinity();
//	}
//	else
//	{
//		rxentry = dxentry / (tempvx*DeltaTime);
//		rxexit = dxexit / (tempvx*DeltaTime);
//	}
//
//	//tính toán t y entry/ exit, tương tự x entry/ exit
//	if (targetVelocity.y == 0.0f)
//	{
//		ryentry = -std::numeric_limits<float>::infinity();
//		ryexit = std::numeric_limits<float>::infinity();
//	}
//	else
//	{
//		ryentry = dyentry / (tempvy*DeltaTime);
//		ryexit = dyexit / (tempvy*DeltaTime);
//	}
//
//	// tính toán thời gian va chạm và thoát khỏi thực sự của vật a chuyển động đối với vật b đứng yên
//	rentry = max(rxentry, ryentry);
//	rexit = min(rxexit, ryexit);
//
//	//sau khi tính toán được thời gian thực sự va chạm và thoát khỏi, ta kiểm tra xem việc va chạm có xảy ra hay không
//	if ((rentry > rexit) || //trường hợp không xảy ra va chạm 1: thời gian thực sự xảy ra va chạm > thời gian thực sự a thoát khỏi b
//
//		(rxentry < 0.0f && ryentry < 0.0f) || //trường hợp không xảy ra va chạm thứ 2: vật a có vận tốc = 0 dẫn đến x entry/ y entry = -vô cùng, hoặc vật a di chuyển hướng ra khỏi vật b
//
//		(rxentry > 1.0f) || (ryentry > 1.0f)) //trường hợp không xảy ra va chạm thứ 3: trong khoảng thời gian delta_time đang xét (thời gian của 1 frame) thì vật a di chuyển chưa tới vật b
//	{
//		return false;
//	}
//	else
//	{
//		return true;
//	}
//
//}


int Collision::IsPreviousTouching(Body *targetBody, Body *otherBody)
{

	float left = otherBody->_PreviousPosition.x - otherBody->GetSize().x / 2 - (targetBody->_PreviousPosition.x + targetBody->GetSize().x / 2);
	float top = otherBody->_PreviousPosition.y + otherBody->GetSize().y / 2 - (targetBody->_PreviousPosition.y - targetBody->GetSize().y / 2);
	float right = otherBody->_PreviousPosition.x + otherBody->GetSize().x / 2 - (targetBody->_PreviousPosition.x - targetBody->GetSize().x / 2);
	float bottom = otherBody->_PreviousPosition.y - otherBody->GetSize().y / 2 - (targetBody->_PreviousPosition.y + targetBody->GetSize().y / 2);

	if (top == 0 || bottom == 0)
	{
		return 1;
	}

	if (left == 0 || right == 0)
	{
		return 2;
	}

	return 0;
}

int Collision::IsTouching(Body *targetBody, Body *otherBody)
{
	float left = otherBody->GetPosition().x - otherBody->GetSize().x / 2 - (targetBody->GetPosition().x + targetBody->GetSize().x / 2);
	float top = otherBody->GetPosition().y + otherBody->GetSize().y / 2 - (targetBody->GetPosition().y - targetBody->GetSize().y / 2);
	float right = otherBody->GetPosition().x + otherBody->GetSize().x / 2 - (targetBody->GetPosition().x - targetBody->GetSize().x / 2);
	float bottom = otherBody->GetPosition().y - otherBody->GetSize().y / 2 - (targetBody->GetPosition().y + targetBody->GetSize().y / 2);

	if (top == 0 || bottom == 0)
	{
		return 1;
	}


	if (left == 0 || right == 0)
	{
		return 2;
	}

	return 0;
}


//void Collision::SetContactListener(WorldContactListener *listener)
//{
//	_Listener = listener;
//}

bool Collision::IsOverlayingRect(const RECT &rect1, const RECT &rect2)
{
	//if it's not overlaying rect
	if (rect1.right < rect2.left || rect1.left > rect2.right ||
		rect1.bottom > rect2.top || rect1.top < rect2.bottom) {
		return false;
	}

	return true;
}

RECT Collision::GetBroadphaseRect(Body *body, float DeltaTime)
{
	// quãng đường đi được sau Deltatime
	Vector2 distance = Vector2(body->GetTotalVelocity().x * DeltaTime, body->GetTotalVelocity().y * DeltaTime);

	RECT rect;
	rect.top = distance.y > 0 ? (body->GetPosition().y + body->GetSize().y / 2) + distance.y : body->GetPosition().y + body->GetSize().y / 2;
	rect.bottom = distance.y > 0 ? body->GetPosition().y - body->GetSize().y / 2 : body->GetPosition().y - body->GetSize().y / 2 + distance.y;
	rect.left = distance.x > 0 ? body->GetPosition().x - body->GetSize().x / 2 : body->GetPosition().x - body->GetSize().x / 2 + distance.x;
	rect.right = distance.x > 0 ? body->GetPosition().x + body->GetSize().x / 2 + distance.x : body->GetPosition().x + body->GetSize().x / 2;

	return rect;
}

RECT Collision::GetRECT(Body *body)
{
	RECT rect;
	rect.top = body->GetPosition().y + body->GetSize().y / 2;
	rect.bottom = body->GetPosition().y - body->GetSize().y / 2;
	rect.left = body->GetPosition().x - body->GetSize().x / 2;
	rect.right = body->GetPosition().x + body->GetSize().x / 2;

	return rect;
}

bool Collision::IsOverlaying(Body *targetBody, Body *otherBody)
{
	moveX = moveY = 0.0f;
	//auto myRect = _target->getBounding();
	//auto otherRect = otherObject->getBounding();

	float left = otherBody->GetPosition().x - otherBody->GetSize().x / 2 - (targetBody->GetPosition().x + targetBody->GetSize().x / 2);
	float top = otherBody->GetPosition().y + otherBody->GetSize().y / 2 - (targetBody->GetPosition().y - targetBody->GetSize().y / 2);
	float right = otherBody->GetPosition().x + otherBody->GetSize().x / 2 - (targetBody->GetPosition().x - targetBody->GetSize().x / 2);
	float bottom = otherBody->GetPosition().y - otherBody->GetSize().y / 2 - (targetBody->GetPosition().y + targetBody->GetSize().y / 2);

	// kt coi có va chạm không
	//  CÓ va chạm khi 
	//  left < 0 && right > 0 && top > 0 && bottom < 0
	//
	if (left >= 0 || right <= 0 || top <= 0 || bottom >= 0)
		return false;

	// tính offset x, y để đi hết va chạm
	// lấy khoảng cách nhỏ nhất
	moveX = abs(left) < right ? left : right;
	moveY = top < abs(bottom) ? top : bottom;

	// chỉ lấy phần lấn vào nhỏ nhất
	if (abs(moveX) < abs(moveY))
		moveY = 0.0f;
	else
		moveX = 0.0f;

	return true;
}

bool Collision::IsPreviousOverlayed(Body *targetBody, Body *otherBody)
{
	float left = otherBody->_PreviousPosition.x - otherBody->GetSize().x / 2 - (targetBody->_PreviousPosition.x + targetBody->GetSize().x / 2);
	float top = otherBody->_PreviousPosition.y + otherBody->GetSize().y / 2 - (targetBody->_PreviousPosition.y - targetBody->GetSize().y / 2);
	float right = otherBody->_PreviousPosition.x + otherBody->GetSize().x / 2 - (targetBody->_PreviousPosition.x - targetBody->GetSize().x / 2);
	float bottom = otherBody->_PreviousPosition.y - otherBody->GetSize().y / 2 - (targetBody->_PreviousPosition.y + targetBody->GetSize().y / 2);

	// kt coi có va chạm không
	//  CÓ va chạm khi 
	//  left < 0 && right > 0 && top > 0 && bottom < 0
	//


	if (left >= 0 || right <= 0 || top <= 0 || bottom >= 0)
		return 0;

	return 1;
}


void Collision::PerformCollision(Body *targetBody, Body *otherBody, float DeltaTime, int collisionAction, bool &needMoveX, bool &needMoveY)
{
	if (targetBody->IsSensor()) return;

	Vector2 targetVelocity = targetBody->GetTotalVelocity();

	if (_CollisionDirection != Vector2(NOT_COLLIDED, NOT_COLLIDED))
	{
		// nếu va chạm theo trục x
		if (_CollisionDirection.x == targetVelocity.x * -1)
		{
			// cập nhật tọa độ
			UpdateTargetPosition(targetBody, Vector2(0, 0));

			// ngăn không cho targetObject di chuyển theo x trong hàm Body->Next trong vòng update World
			needMoveX = false;
		}
		else
		{
			//nếu va chạm theo trục y
			if (_CollisionDirection.y == targetVelocity.y * -1)
			{
				// cập nhật tọa độ
				UpdateTargetPosition(targetBody, Vector2(0, 0));

				// ngăn không cho targetObject di chuyển theo y trong hàm Body->Next trong vòng update World
				needMoveY = false;
			}
		}
	}
}


void Collision::UpdateTargetPosition(Body *targetBody,const Vector2& move)
{
	//if (targetBody->IsSensor()) return;

	if (move.x == 0 && move.y == 0)
		targetBody->SetPosition(_CollisionPosition.x, _CollisionPosition.y);
	else {
		targetBody->SetPosition(targetBody->GetPosition().x + move.x, targetBody->GetPosition().y + move.y);
	}
}

void  Collision::PerformOverlaying(Body *targetBody, Body* otherBody, bool &needMoveX, bool &needMoveY)
{
	/*if (!targetBody->_IsSensor)
	{*/
	if (moveX != 0) //nếu cần chỉnh lại toạ độ x thì không cho obj thay đổi x trong Body->Next
		needMoveX = false;
	if (moveY != 0) //nếu cần chỉnh lại toạ độ y thì không cho obj thay đổi y trong Body->Next
		needMoveY = false;
	//}

	//if (targetBody->_IsSensor)
	//{
	//	if (!IsPreviousOverlayed(targetBody, otherBody))
	//	{
	//		if (!IsSensorEntered)
	//		{
	//			//Console::Log("Hello");
	//			_Listener->OnSersorEnter(targetBody, otherBody);
	//		}
	//	}
	//	else
	//	{
	//		_Listener->OnSersorOverlaying(targetBody, otherBody);
	//	}
	//}

	UpdateTargetPosition(targetBody, Vector2(moveX, moveY));

}

void Collision::Reset()
{
	IsSensorEntered = false;


}

void Collision::Push(Body *object)
{
	//float magnitude = (sqrt(object.GetTotalVelocity(DeltaTime).x * object.GetTotalVelocity(DeltaTime).x + object.GetTotalVelocity(DeltaTime).y * object.GetTotalVelocity(DeltaTime).y)) * _RemainingTime;
	//float dotprod = object.GetTotalVelocity(DeltaTime)().X * _CollisionDirection.X + object.GetTotalVelocity(DeltaTime)().Y * _CollisionDirection.Y;
	/*if (dotprod > 0.0f)
	dotprod = 1.0f;
	else if (dotprod < 0.0f)
	dotprod = -1.0f;*/
	//Vector2 NewObjectVelocity = Vector2(dotprod * _CollisionDirection.Y * magnitude, dotprod * _CollisionDirection.X * magnitude);
	//object.SetVelocity(NewObjectVelocity);
}

void Collision::Slide(Body *object)
{
	/*float dotprod = (object.GetTotalVelocity(DeltaTime)().X * _CollisionDirection.X + object.GetTotalVelocity(DeltaTime)().Y * _CollisionDirection.Y) * _RemainingTime;
	Vector2 NewObjectVelocity = Vector2(dotprod * _CollisionDirection.Y, dotprod * _CollisionDirection.X);*/
	/*object.SetVelocity(NewObjectVelocity);*/
}

void Collision::Deflect(Body *object)
{
	/*Vector2 RemainingVelocity = Vector2(object.GetTotalVelocity(DeltaTime)().X * _RemainingTime, object.GetTotalVelocity(DeltaTime)().Y * _RemainingTime);
	if (abs(_CollisionDirection.X) > 0.0001f)
	{
	RemainingVelocity.X = -RemainingVelocity.X;
	}
	if (abs((_CollisionDirection.Y) > 0.0001f))
	{
	RemainingVelocity.Y = -RemainingVelocity.Y;
	}
	object.SetVelocity(RemainingVelocity);*/
}