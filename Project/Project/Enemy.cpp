#include"Enemy.h"
#include"DirectXDevice.h"
Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	//delete(pol);
}

Enemy::Enemy(const Enemy * ene)
{
}

///<summary>
///������
///</summary>
void Enemy::Initialize()
{
	col = new CircleCollision();
	col->scale = 10;
	//col->length = 10;
	ai.Initialize();
	SetState();
	Deadflag = false;
	Endflag = false;
	Deadflag = false;
	obj->Initialize();
	obj->LoadObj("UFO");
	obj->position.y = 0;
	obj2->Initialize();
	obj2->LoadObj("UFO_Red");
	obj2->position.y = 0;
}

///<summary>
///�X�V����
///</summary>
void Enemy::Update()
{
	if (!Deadflag)
	{
		if (Endflag)
		{
			endscale = obj->scale;
			time++;
			if (time <= 10)
			{
				endscale.x += 0.5f;
				endscale.y += 0.5f;
				endscale.z += 0.5f;
			}
			if (time > 10)
			{
				endscale.x -= 0.5f;
				endscale.y -= 0.5f;
				endscale.z -= 0.5f;
			}
			if (endscale.x <= 0)
			{
				endscale = XMFLOAT3(0, 0, 0);
				Deadflag = true;
			}

			obj->SetScale(endscale);
			obj->SetScale(endscale);
		}
		//pol->Update();
		obj->Update();

		obj2->Update();
		obj2->SetPos(obj->position);
		col->position = obj->position;
		PositionUpdate(Ancer1, Ancer2, mokuhyou->GetPosition());
		GetState();
		GetAlive();
	}
}


///<summary>
///�`�揈��
///</summary>
void Enemy::Draw(ID3D12GraphicsCommandList * cmdList)
{
	if (!Deadflag)
	{

		if (Damege)
		{
			time++;
			if (time <= endtime)
			{
				obj2->Draw(DirectXDevice::cmdList);
			}
			else
			{
				time = 0;
				obj->Draw(DirectXDevice::cmdList);
				Damege = false;
			}
		}
		else
		{
			//pol->Draw(DirectXDevice::cmdList, DirectXDevice::dev);
			obj->Draw(DirectXDevice::cmdList);
		}
	}
}

///<summary>
///�ʒu�ݒ�
///</summary>
void Enemy::SetPos(XMFLOAT3 position)
{
	//pol->position = position;
	obj->position = position;
	obj2->position = position;
}

///<summary>
///�傫���ݒ�
///</summary>
void Enemy::SetScale(XMFLOAT3 scale)
{
	//pol->scale = scale;
	obj->scale = scale;
	obj2->scale = scale;
}

///<summary>
///��X�K�v�Ȃ��Ȃ�΍폜���܂��B
///</summary>
void Enemy::Install(EnemyAI Ai)
{
	ai = Ai;
}

///<summary>
///�̗́A�U���͂�ݒ肵�Ă܂��B
///</summary>
void Enemy::SetState()
{
	Hp = 5.0f;
	Speed = GetSpeed();
	Power = ai.GetPower();
}

///<summary>
///�ʒu�X�V���]�T������΁A�o�H�T���Ɉڍs���܂��B�O�_�ڕW�����ύX�ɂȂ�\���������ӂ��������B
///<param name="pointA">��1�ړ��n�_</param>
///<param name="pointB">��2�ړ��n�_</param>
///<param name="tower">�ŏI�ڕW�n�_</param>
///</summary>
void Enemy::PositionUpdate(XMFLOAT3 pointA, XMFLOAT3 pointB, XMFLOAT3 tower)//�G�l�~�[�̍s���ł��B
{
	if (Hp <= 0)
	{
		state = Destory;
		if (Dead)
		{
			return;
		}
		time = 0;
		Dead = true;
	}

	switch (state)
	{
	    case Destory:
#pragma region
		Endflag = true;
		break;
#pragma endregion
		case Stay:
#pragma region
			break;
#pragma endregion
		case move1://��1�ړ�
#pragma region
			if (obj->position.x - pointA.x > 0.5f) {
				obj->position.x = obj->position.x + vel.z;
			}
			else if (obj->position.x - pointA.x < -0.5f)
			{				
				obj->position.x = obj->position.x + vel.x;
			}
			else
			{
				NextX = true;
			}
			if (NextX) {
				if (obj->position.z - pointA.z > 0.5f)
				{
					obj->position.z = obj->position.z + vel.z;
				}
				else if (obj->position.z - pointA.z < -0.5)
				{
					obj->position.z = obj->position.z + vel.x;
				}
				else
				{
					NextZ = true;
				}
			}
			if (NextX == true, NextZ == true)
			{
				state = move2;
				NextX = false;
				NextZ = false;
			}
			break;
#pragma endregion
		case move2://��2�ړ�
#pragma region
			if (obj->position.x - pointB.x > 0.5f) {
				obj->position.x = obj->position.x + vel.z;
			}
			else if (obj->position.x - pointB.x < -0.5f)
			{
				obj->position.x = obj->position.x + vel.x;
			}
			else
			{
				NextX = true;
			}
			if (NextX) {
				if (obj->position.z - pointB.z > 0.5f)
				{
					obj->position.z = obj->position.z + vel.z;
				}
				else if (obj->position.z - pointB.z < -0.5)
				{
					obj->position.z = obj->position.z + vel.x;
				}
				else
				{
					NextZ = true;
				}
			}
			if (NextX == true, NextZ == true)
			{
				state = move3;
				NextX = false;
				NextZ = false;
			}
			break;
#pragma endregion
		case move3://��3�ړ�
#pragma region
			if (obj->position.x - tower.x > 0.5f) {
				obj->position.x = obj->position.x + vel.z;
			}
			else if (obj->position.x - tower.x < -0.5f)
			{
				obj->position.x = obj->position.x + vel.x;
			}
			else
			{
				NextX = true;
			}
			if (NextX) {
				if (obj->position.z - tower.z > 0.5f)
				{
					obj->position.z = obj->position.z + vel.z;
				}
				else if (obj->position.z - tower.z < -0.5)
				{
					obj->position.z = obj->position.z + vel.x;
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
		case atack1://�ڕW���ɑ΂��Ă̍U��
#pragma region

			if (Cnt < 10.0f)
			{
				Cnt++;
				obj->position.y = obj->position.y + vel.z;
			}
			else if (Cnt > 20.0f)
			{
				Cnt = 0;
				TowerAtack();
			}
			else if (Cnt <= 20.0f &Cnt >= 10.0f)
			{
				Cnt++;
				obj->position.y = obj->position.y - vel.z;
			}
			break;
#pragma endregion
		case special1://�G�l�~�[���Ƃ̓���s�����s�킹��B
#pragma region
			Avoid();
			break;
#pragma endregion
	}
	obj->position;
}

///<summary>
///��ԕω��A�s��
///</summary>
State Enemy::GetState()
{
	return State();
}

///<summary>
///�G�l�~�[���Ƃɐݒ肳�ꂽ����ȍs�����s���܂��B
///</summary>
void Enemy::Avoid()
{
	//�ڐG���肪�Ȃ��Ȃ������_
	switch (code) 
	{//�s�����O�ɂ���čs�����ω����܂��B
	case Lazy://�ʓ|�L����c�c�c�ŒZ���[�g��i�݂܂��B
#pragma region

		break;
#pragma endregion
	case Pride://���ʂڂ�c�c�c��{�I�ɓG�����Ȃ����ɐi�ތX�����������܂��B
#pragma region
		break;
#pragma endregion
	case Coward://�������c�c�c�cDamege��TRUE�ɂȂ莟�����s�����J�n���܂��B
#pragma region
		if (Damege&&!Kosinuke)
		{
			Kosinuke = true;
		}
		if (Kosinuke)
		{
			//��������ԂȂ̂ōs���𐧌����܂��B
		}
		break;
#pragma endregion
	case Sage://�m�b�ҁc�c�c�c��{�I�ɓG���������ɐi�ތX�����������܂��B
#pragma region
		break;
	}

	#pragma endregion

}

///<summary>
///�ڕW�ʒu�����������Ă���̂Ɏg�p���܂��B
///<param name="x">�ڕW�n�_</param>
///</summary>
void Enemy::SetTarget(XMFLOAT3* x){TargetTower = *x;}

///<summary>
///��ɍU���������ɕύX���܂��B
///<param name="tow">�ŏI�ڕW</param>
///</summary>
void Enemy::SetTower(Tower* tow){mokuhyou = tow;}

///<summary>
///�^���[�ɑ΂��Ă̍U�����s���܂��B
///</summary>
void Enemy::TowerAtack()
{
	//mokuhyou->SetHp(GetPower());
	mokuhyou->Damage(GetPower());
	Hp = 0;
}

///<summary>
///���S����(�\��n�ł�)
///</summary>
void Enemy::GetAlive()
{
	if (Deadflag)
	{
	}
}

///<summary>
///�G�l�~�[�̐��i���������Ă��܂��B
///</summary>
CodeOfConduct Enemy::GetCode(){return code;}

///<summary>
///�G�l�~�[�ɑ΂���U�������A�^����_���[�W��float�`���œ��͂���B
///<param name="x">�_���[�W��</param>
///</summary>
void Enemy::EnemyDamege(float x)
{
	Hp = Hp - x;
	if (!Damege)
	{
		Damege = true;
	}
}

///<summary>
///�G�l�~�[��1�ڕW�ݒ�
///</summary>
XMFLOAT3 Enemy::SetAncer1(XMFLOAT3 x)
{
	Ancer1 = x;
	return Ancer1;
}


///<summary>
///�G�l�~�[��2�ڕW�ݒ�
///</summary>
XMFLOAT3 Enemy::SetAncer2(XMFLOAT3 x)
{
	Ancer2 = x;
	return Ancer2;
}

///<summary>
///���ڕW�ǐՂɖ߂�
///</summary>
void Enemy::ActionRiset()
{
	state = move1;
}

XMFLOAT3 Enemy::GetPosition()
{
	return obj->position;
}
void Enemy::SpeedEne()
{
	vel = { 0.1f + (Speed / 10.0f), 0, -0.1f - (Speed / 10.0f) };
}
float Enemy::GetHp()
{
	return Hp;
}

void Enemy::SetHp(float x)
{
	Hp = x;
}

float Enemy::GetSpeed()
{
	return Speed;
}

void Enemy::SetSpeed(float x)
{
	Speed = x/10;
	vel = { Speed,0,-Speed };
}