#include"Enemy.h"
#include"DirectXDevice.h"
Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	delete(pol);
}

///<summary>
///������
///</summary>
void Enemy::Initialize()
{
	pol->Initialize(DirectXDevice::dev);
	ai.Initialize();
	SetState();
	Deadflag = false;
}

///<summary>
///�X�V����
///</summary>
void Enemy::Update()
{
	if (Deadflag)
	{
		return;
	}
	pol->Update();
	PositionUpdate(XMFLOAT3{ 15,15,15 }, XMFLOAT3{ 10,10,10 }, TargetTower);
	GetState();
	GetAlive();
}


///<summary>
///�`�揈��
///</summary>
void Enemy::Draw()
{
	if (Deadflag)
	{
		return;
	}
	pol->Draw(DirectXDevice::cmdList, DirectXDevice::dev);
}

///<summary>
///�ʒu�ݒ�
///</summary>
void Enemy::SetPos(XMFLOAT3 position)
{
	pol->position = position;
}

///<summary>
///�傫���ݒ�
///</summary>
void Enemy::SetScale(XMFLOAT3 scale)
{
	pol->scale = scale;
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
	Hp = ai.GetHp();
	Speed = ai.GetSpeed();
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
	}
	switch (state)
	{
	    case Destory:
#pragma region
		Deadflag = true;
		break;
#pragma endregion
		case move1://��1�ړ�
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
				//pol->position.y = pol->position.y + Floar2;
				NextX = false;
				NextZ = false;
			}
			break;
#pragma endregion
		case move2://��2�ړ�
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
				state = move3;
				//pol->position.y = pol->position.y - Floar2;
				NextX = false;
				NextZ = false;
			}
			break;
#pragma endregion
		case move3://��3�ړ�
#pragma region
			if (!NextX) {
				if (pol->position.x - tower.x > 0.5f) {
					pol->position.x = pol->position.x + vel.x;
				}
				else if (pol->position.x - tower.x < -0.5f)
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
		case atack1://�ڕW���ɑ΂��Ă̍U��
#pragma region

			if (Cnt < 120.0f)
			{
				Cnt++;
				pol->position.y = pol->position.y + vel.z;
			}
			else if (Cnt > 240.0f)
			{
				Cnt = 0;
				TowerAtack();
			}
			else if (Cnt <= 240.0f &Cnt >= 120.0f)
			{
				Cnt++;
				pol->position.y = pol->position.y - vel.z;
			}
			break;
#pragma endregion
		case special1://�G�l�~�[���Ƃ̓���s�����s�킹��B
#pragma region
			Avoid();
			break;
#pragma endregion
	}
		pol->position;
}

///<summary>
///��ԕω��A�s��
///</summary>
State Enemy::GetState()
{
	return State();
}

///<summary>
///��Q��������\��n
///</summary>
void Enemy::Avoid()//��Q��������܂�
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
		break;
#pragma endregion
	case Sage://�m�b�ҁc�c�c�c��{�I�ɓG���������ɐi�ތX�����������܂��B
#pragma region
		break;
	}

	#pragma endregion
	//if(/*�����蔻��������܂�*/)
		//�E�����ւ̉I���
		//if(/*�Ăє��肪TRUE�̏ꍇ*/)
			//�������ւ̉I������܂���Avoid�̍s�����I�������܂��B
}

///<summary>
///�ڕW�ʒu�����������Ă���̂Ɏg�p���܂��B
///<param name="x">�ڕW�n�_</param>
///</summary>
void Enemy::SetTarget(XMFLOAT3* x)
{
	TargetTower = *x;
}

///<summary>
///��ɍU���������ɕύX���܂��B
///<param name="tow">�ŏI�ڕW</param>
///</summary>
void Enemy::SetTower(Tower* tow)
{
	mokuhyou = tow;
}

///<summary>
///�^���[�ɑ΂��Ă̍U�����s���܂��B
///</summary>
void Enemy::TowerAtack()
{
	//mokuhyou->SetHp(GetPower());
	mokuhyou->hp = mokuhyou->hp - GetPower();
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
CodeOfConduct Enemy::GetCode()
{
	return code;
}

///<summary>
///�G�l�~�[�ɑ΂���U�������A�^����_���[�W��float�`���œ��͂���B
///<param name="x">�_���[�W��</param>
///</summary>
void Enemy::EnemyDamege(float x)
{
	SetHp(Hp-x);
	if (Damege)
	{
		return;
	}
	Damege = true;
}


