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
	//	����K�̈ړ��|�C���g�A3�K�̈ړ��|�C���g�A�ڕW�̏��Ԃɍ��W�w�肵�Ă��܂��B���͌�X�Ή������܂��B
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

void Enemy::PositionUpdate(XMFLOAT3 pointA, XMFLOAT3 pointB, XMFLOAT3 tower)//�G�l�~�[�̍s���ł��B
{

	switch (state)
	{
	case move1://1�K�w�ړ�
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
	case move2://2�K�w�ړ�
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
	case move3://3�K�w�ړ�
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

void Enemy::Avoid()//��Q��������܂�
{
			//�ڐG���肪�Ȃ��Ȃ������_
	//switch(CodeOfConduct)//�s�����O�ɂ���čs�����ω����܂��B
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
	//if(/*�����蔻��������܂�*/)
		//�E�����ւ̉I���
		//if(/*�Ăє��肪TRUE�̏ꍇ*/)
			//�������ւ̉I������܂���Avoid�̍s�����I�������܂��B


}

void Enemy::SetTarget(XMFLOAT3* x)
{
	TargetTower = *x;
}


