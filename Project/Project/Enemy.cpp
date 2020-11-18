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
	case move1://1階層移動
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
	case move2://2階層移動
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
	case move3://3階層移動
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
	//switch(CodeOfConduct)//行動理念によって行動が変化します。
	//if(/*あたり判定を書きます*/)
		//右方向への迂回を
		//if(/*再び判定がTRUEの場合*/)
			//左方向への迂回をします
			//接触判定がなくなった時点でAvoidの行動を終了させます。

}


