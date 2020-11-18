#include"Enemy.h"
#include"DirectXDevice.h"
Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	delete(pol);
}

void Enemy::Initialize()
{
	pol->Initialize(DirectXDevice::dev);
	ai.Initialize();
	SetState();
}

void Enemy::Update()
{
	pol->Update();
	//vel = //ai.UpdateAI();
	PositionUpdate(XMFLOAT3{ 15,15,15 }, XMFLOAT3{ -15,-15,-15 }, XMFLOAT3{ 0,0,0 });
	//UpdateAI(XMFLOAT3{ 15,15,15 }, XMFLOAT3{ -15,-15,-15 }, XMFLOAT3{ 0,0,0 });
}



void Enemy::Draw()
{
	pol->Draw(DirectXDevice::cmdList, DirectXDevice::dev);
}

void Enemy::SetPos(XMFLOAT3 position)
{
	pol->position = position;
}

void Enemy::SetScale(XMFLOAT3 scale)
{
	pol->scale = scale;
}

void Enemy::Install(EnemyAI Ai)
{
	ai = Ai;
}

void Enemy::SetState()
{
	Hp = ai.GetHp();
	Speed = ai.GetSpeed();
	Power = ai.GetPower();
}

void Enemy::PositionUpdate(XMFLOAT3 pointA, XMFLOAT3 pointB, XMFLOAT3 tower)
{
	switch (state)
	{
	case move1://1ŠK‘wˆÚ“®
#pragma region
		if (pol->position.x - pointA.x > 0.5f) {
			pol->position.x = pol->position.x + vel.x;
		}
		else if (pol->position.x - pointA.x < -0.5f)
		{
			pol->position.x = pol->position.x + vel.z;
		}
		else
		{
			NextX = true;
		}
		pol->position.y = pol->position.y + vel.y;
		if (pol->position.z - pointA.z > 0.5f)
		{
			pol->position.z = pol->position.z + vel.x;
		}
		else if (pol->position.z - pointA.z < -0.5)
		{
			pol->position.x = pol->position.x + vel.z;
			NextZ = true;
		}
		else
		{
			NextZ = true;
		}
		if (NextX == true, NextZ == true)
		{
			state = move2;
			pol->position.y = pol->position.y + Floar2;
			NextX = false;
			NextZ = false;
		}
		break;
#pragma endregion
	case move2://2ŠK‘wˆÚ“®
#pragma region
		if (!NextX) {
			if (pol->position.x > tower.x) {
				pol->position.x = pol->position.x + vel.x;
			}
			else if (pol->position.x < tower.x)
			{
				pol->position.x = pol->position.x + vel.z;
			}
			else
			{
				NextX = true;
			}
		}
		pol->position.y = pol->position.y + vel.y;
		if (!NextZ) {
			if (pol->position.z > tower.z)
			{
				pol->position.z = pol->position.z + vel.x;
			}
			else if (pol->position.z < tower.z)
			{
				pol->position.x = pol->position.x + vel.z;
			}
			else
			{
				NextZ = true;
			}
		}
		if (NextX == true, NextZ == true)
		{
			state = atack1;
			NextX = false;
			NextZ = false;
		}
		break;
#pragma endregion
	case move3://3ŠK‘wˆÚ“®
#pragma region
		if (pol->position.x > pointB.x) {
			pol->position.x = pol->position.x + vel.x;
		}
		else if (pol->position.x < pointB.x)
		{
			pol->position.x = pol->position.x + vel.z;
		}
		else
		{
			NextX = true;
		}
		pol->position.y = pol->position.y + vel.y;
		if (pol->position.z >  pointB.z)
		{
			pol->position.z = pol->position.z + vel.x;
		}
		else if (pol->position.z < pointB.z)
		{
			pol->position.x = pol->position.x + vel.z;
		}
		else
		{
			NextZ = true;
		}
		if (NextX == true, NextZ == true)
		{
			state = move2;
			NextX = false;
			NextZ = false;
		}
		break;
#pragma endregion
	case atack1:
#pragma region
		break;
#pragma endregion
	case atack2:
#pragma region
		break;
#pragma endregion
	case Destory:
#pragma region
		break;
#pragma endregion
	}
	pol->position;
}
State Enemy::GetState()
{
	return State();
}

void Enemy::Avoid()
{
			//ÚG”»’è‚ª‚È‚­‚È‚Á‚½“_
	//switch(CodeOfConduct)//s“®—”O‚É‚æ‚Á‚Äs“®‚ª•Ï‰»‚µ‚Ü‚·B
	//case 0:
	//#pragma region
	//break;
	//#pragma endregion
	//case 1:
	//#pragma region
	//break;
	//#pragma endregion
	//case 2:
	//#pragma region
	//break;
	//#pragma endregion
	//case 3:
	//#pragma region
	//break;
	//#pragma endregion
	//if(/*‚ ‚½‚è”»’è‚ğ‘‚«‚Ü‚·*/)
		//‰E•ûŒü‚Ö‚Ì‰I‰ñ‚ğ
		//if(/*Ä‚Ñ”»’è‚ªTRUE‚Ìê‡*/)
			//¶•ûŒü‚Ö‚Ì‰I‰ñ‚ğ‚µ‚Ü‚·‚ÅAvoid‚Ìs“®‚ğI—¹‚³‚¹‚Ü‚·B


}


