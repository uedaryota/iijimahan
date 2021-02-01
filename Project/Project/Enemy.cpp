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
///初期化
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
///更新処理
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
///描画処理
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
///位置設定
///</summary>
void Enemy::SetPos(XMFLOAT3 position)
{
	//pol->position = position;
	obj->position = position;
	obj2->position = position;
}

///<summary>
///大きさ設定
///</summary>
void Enemy::SetScale(XMFLOAT3 scale)
{
	//pol->scale = scale;
	obj->scale = scale;
	obj2->scale = scale;
}

///<summary>
///後々必要なくなれば削除します。
///</summary>
void Enemy::Install(EnemyAI Ai)
{
	ai = Ai;
}

///<summary>
///体力、攻撃力を設定してます。
///</summary>
void Enemy::SetState()
{
	Hp = 5.0f;
	Speed = GetSpeed();
	Power = ai.GetPower();
}

///<summary>
///位置更新→余裕があれば、経路探索に移行します。三点目標制が変更になる可能性をご留意ください。
///<param name="pointA">第1移動地点</param>
///<param name="pointB">第2移動地点</param>
///<param name="tower">最終目標地点</param>
///</summary>
void Enemy::PositionUpdate(XMFLOAT3 pointA, XMFLOAT3 pointB, XMFLOAT3 tower)//エネミーの行動です。
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
		case move1://第1移動
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
		case move2://第2移動
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
		case move3://第3移動
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
		case atack1://目標物に対しての攻撃
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
		case special1://エネミーごとの特殊行動を行わせる。
#pragma region
			Avoid();
			break;
#pragma endregion
	}
	obj->position;
}

///<summary>
///状態変化、行動
///</summary>
State Enemy::GetState()
{
	return State();
}

///<summary>
///エネミーごとに設定された特殊な行動を行います。
///</summary>
void Enemy::Avoid()
{
	//接触判定がなくなった時点
	switch (code) 
	{//行動理念によって行動が変化します。
	case Lazy://面倒臭がり………最短ルートを進みます。
#pragma region

		break;
#pragma endregion
	case Pride://うぬぼれ………基本的に敵が少ない道に進む傾向を持たせます。
#pragma region
		break;
#pragma endregion
	case Coward://腰抜け…………DamegeがTRUEになり次第特殊行動を開始します。
#pragma region
		if (Damege&&!Kosinuke)
		{
			Kosinuke = true;
		}
		if (Kosinuke)
		{
			//腰抜け状態なので行動を制限します。
		}
		break;
#pragma endregion
	case Sage://知恵者…………基本的に敵が多い道に進む傾向を持たせます。
#pragma region
		break;
	}

	#pragma endregion

}

///<summary>
///目標位置を引っ張ってくるのに使用します。
///<param name="x">目標地点</param>
///</summary>
void Enemy::SetTarget(XMFLOAT3* x){TargetTower = *x;}

///<summary>
///主に攻撃処理等に変更します。
///<param name="tow">最終目標</param>
///</summary>
void Enemy::SetTower(Tower* tow){mokuhyou = tow;}

///<summary>
///タワーに対しての攻撃を行います。
///</summary>
void Enemy::TowerAtack()
{
	//mokuhyou->SetHp(GetPower());
	mokuhyou->Damage(GetPower());
	Hp = 0;
}

///<summary>
///死亡処理(予定地です)
///</summary>
void Enemy::GetAlive()
{
	if (Deadflag)
	{
	}
}

///<summary>
///エネミーの性格を引っ張てきます。
///</summary>
CodeOfConduct Enemy::GetCode(){return code;}

///<summary>
///エネミーに対する攻撃処理、与えるダメージをfloat形式で入力する。
///<param name="x">ダメージ量</param>
///</summary>
void Enemy::EnemyDamege(float x)
{
	Hp = Hp - x;
	if (!Endflag)
	{
		if (!Damege)
		{
			Damege = true;
		}
	}
}

///<summary>
///エネミー第1目標設定
///</summary>
XMFLOAT3 Enemy::SetAncer1(XMFLOAT3 x)
{
	Ancer1 = x;
	return Ancer1;
}


///<summary>
///エネミー第2目標設定
///</summary>
XMFLOAT3 Enemy::SetAncer2(XMFLOAT3 x)
{
	Ancer2 = x;
	return Ancer2;
}

///<summary>
///第一目標追跡に戻す
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