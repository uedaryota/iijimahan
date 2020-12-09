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
///初期化
///</summary>
void Enemy::Initialize()
{
	pol->Initialize(DirectXDevice::dev);
	ai.Initialize();
	SetState();
	Deadflag = false;
}

///<summary>
///更新処理
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
///描画処理
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
///位置設定
///</summary>
void Enemy::SetPos(XMFLOAT3 position)
{
	pol->position = position;
}

///<summary>
///大きさ設定
///</summary>
void Enemy::SetScale(XMFLOAT3 scale)
{
	pol->scale = scale;
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
	Hp = ai.GetHp();
	Speed = ai.GetSpeed();
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
	}
	switch (state)
	{
	    case Destory:
#pragma region
		Deadflag = true;
		break;
#pragma endregion
		case move1://第1移動
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
		case move2://第2移動
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
		case move3://第3移動
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
		case atack1://目標物に対しての攻撃
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
		case special1://エネミーごとの特殊行動を行わせる。
#pragma region
			Avoid();
			break;
#pragma endregion
	}
		pol->position;
}

///<summary>
///状態変化、行動
///</summary>
State Enemy::GetState()
{
	return State();
}

///<summary>
///障害物避ける予定地
///</summary>
void Enemy::Avoid()//障害物を避けます
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
		break;
#pragma endregion
	case Sage://知恵者…………基本的に敵が多い道に進む傾向を持たせます。
#pragma region
		break;
	}

	#pragma endregion
	//if(/*あたり判定を書きます*/)
		//右方向への迂回を
		//if(/*再び判定がTRUEの場合*/)
			//左方向への迂回をしますでAvoidの行動を終了させます。
}

///<summary>
///目標位置を引っ張ってくるのに使用します。
///<param name="x">目標地点</param>
///</summary>
void Enemy::SetTarget(XMFLOAT3* x)
{
	TargetTower = *x;
}

///<summary>
///主に攻撃処理等に変更します。
///<param name="tow">最終目標</param>
///</summary>
void Enemy::SetTower(Tower* tow)
{
	mokuhyou = tow;
}

///<summary>
///タワーに対しての攻撃を行います。
///</summary>
void Enemy::TowerAtack()
{
	//mokuhyou->SetHp(GetPower());
	mokuhyou->hp = mokuhyou->hp - GetPower();
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
CodeOfConduct Enemy::GetCode()
{
	return code;
}

///<summary>
///エネミーに対する攻撃処理、与えるダメージをfloat形式で入力する。
///<param name="x">ダメージ量</param>
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


