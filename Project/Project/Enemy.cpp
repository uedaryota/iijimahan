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
	PositionUpdate(XMFLOAT3{ 15,15,15 }, XMFLOAT3{ 15,15,15 }, TargetTower);
	//	ªˆêŠK‚ÌˆÚ“®ƒ|ƒCƒ“ƒgA3ŠK‚ÌˆÚ“®ƒ|ƒCƒ“ƒgA–Ú•W‚Ì‡”Ô‚ÉÀ•WŽw’è‚µ‚Ä‚¢‚Ü‚·B”‚ÍŒãX‘Î‰ž‚³‚¹‚Ü‚·B
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

void Enemy::PositionUpdate(XMFLOAT3 pointA, XMFLOAT3 pointB, XMFLOAT3 tower)//ƒGƒlƒ~[‚Ìs“®‚Å‚·B
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
		if (NextX) {
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
			if (pol->position.x - tower.x > 0.5f) {
				pol->position.x = pol->position.x + vel.x;
			}
			else if (pol->position.x - tower.x<-0.5f)
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
			if (pol->position.z - tower.z > 0.5f)
			{
				pol->position.z = pol->position.z + vel.x;
			}
			else if (pol->position.z - tower.z < -0.5f)
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
		if (NextZ) {
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
			pol->position.y = pol->position.y - Floar2;
			NextX = false;
			NextZ = false;
		}
		break;
#pragma endregion
	case atack1:
#pragma region
			
		if (Cnt < 120.0f)
		{
			Cnt++;
			pol->position.y = pol->position.y + vel.z;
		}
		else if(Cnt > 240.0f)
		{
			Cnt = 0;
			TowerAtack();
		}
		else if(Cnt<=240.0f &Cnt>=120.0f)
		{
			Cnt++;
			pol->position.y = pol->position.y - vel.z;
		}
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

void Enemy::Avoid()//áŠQ•¨‚ð”ð‚¯‚Ü‚·
{
			//ÚG”»’è‚ª‚È‚­‚È‚Á‚½Žž“_
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
	//if(/*‚ ‚½‚è”»’è‚ð‘‚«‚Ü‚·*/)
		//‰E•ûŒü‚Ö‚Ì‰I‰ñ‚ð
		//if(/*Ä‚Ñ”»’è‚ªTRUE‚Ìê‡*/)
			//¶•ûŒü‚Ö‚Ì‰I‰ñ‚ð‚µ‚Ü‚·‚ÅAvoid‚Ìs“®‚ðI—¹‚³‚¹‚Ü‚·B


}

void Enemy::SetTarget(XMFLOAT3* x)
{
	TargetTower = *x;
}

void Enemy::SetTower(Tower* tow)
{
	mokuhyou = tow;
}

void Enemy::TowerAtack()
{
	//mokuhyou->SetHp(GetPower());
	mokuhyou->hp = mokuhyou->hp - GetPower();
}


