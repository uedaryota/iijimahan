#include "GamePlay.h"
#include"DirectXDevice.h"

#include "Input.h"
#include"Texture.h"
#include"Stage2D.h"
#include"Battery.h"
#include"Collision.h"
#include"Texture.h"
#include"PMDClass.h"
#include"Floor.h"
#include"Camera.h"
#include"Poly.h"
#include"Sprite.h"
#include"Block.h"
#include"Tower.h"
#include"Sound.h"
#include"Stage.h"
#include"Enemy.h"
#include"EnemyManeger.h"
#include"Spawn.h"
Camera* c = new Camera();
PMDClass* pmd = new PMDClass();
Floor* floor1 = new Floor();
Tower* tower = new Tower();
XMFLOAT3 pointA = { 15,15,15 };
XMFLOAT3 pointB = { -15,-15,-15 };
Block* block = new Block();
Sound* sound = new Sound();
//Stage* stage = new Stage();
Stage2D* stage = new Stage2D();
Enemy* enemy = new Enemy();
Enemy* enemy2 = new Enemy();
EnemyAI* ai = new EnemyAI();
EnemyManeger* manager = new EnemyManeger();
Sprite* back = new Sprite();
Input* input = new Input();
Texture* tex = new Texture();
Battery* bat = new Battery();
Collision* collider = new Collision();
Spawn* spawn = new Spawn();

float timer = 0;
float spawntime = 10;
int EneMax = 3;
int EneNow = 0;
void GamePlay::Update()
{
	input->Update();

	if (input->PushKey(DIK_P))
	{
		sound->PlayRoop();
	}
	Camera::Update();
	tower->Update();
	spawn->Update();
	//	sound->Update();
	if (EneNow < EneMax)
	{
		timer++;
		if (timer / 60 > spawntime)
		{
			manager->Add2(spawn->GetPosition());
			manager->ReAncerSet(XMFLOAT3{ -100,1,-100 }, XMFLOAT3{ 500,500,500 });
			manager->SetTowerEnemy(tower);
			timer = 0;
			EneNow++;
		}
	}
	

	enemy->SetScale(XMFLOAT3{ 10,10,10 });
	manager->Update();
	//enemy->Update();
	//enemy->Draw(DirectXDevice::cmdList);
	/*back->Update();
	back->Draw();
	*/
	stage->Update();

	bat->Update();


	//if (input->PushKey(DIK_Q))//実験用→実験結果成功　＊座標の変更を行えます。
	//{
	//	manager->AncerSet(XMFLOAT3{ 15,15,15 }, XMFLOAT3{ 15,15,15 });
	//}
	if (input->PushKey(DIK_SPACE))//実験用→実験結果成功　＊座標の変更を行えます。
	{
		manager->ReAncerSet(XMFLOAT3{ -100,1,-100 }, XMFLOAT3{ 500,500,500 });
	}
	if (input->TriggerKey(DIK_Z))
	{
		enemy->EnemyDamege(1);
		//enemy2->EnemyDamege(0.5);
	}

	CollisionUpdate();
}
GamePlay::GamePlay()
{
}
void GamePlay::Draw()
{
	tower->Draw(DirectXDevice::cmdList);
	spawn->Draw(DirectXDevice::cmdList);
	manager->Draw(DirectXDevice::cmdList);
	stage->Draw();
	bat->Draw();
}
void GamePlay::Initialize()
{
	block->Initialize();
	tower->Initialize(DirectXDevice::dev);

	floor1->Initialize(DirectXDevice::dev);
	manager->Initialize();
	enemy->Initialize();
	sound->Initialize();
	stage->Initialize();
	spawn->Initialize(DirectXDevice::dev);
	//Set = &SetAd;
	spawn->SetSpawn(10, 10);
	manager->Add2(spawn->GetPosition());
	//manager->Add(enemy);
	//manager->Add(enemy2);
	enemy->state = move1;
	timer = 0;
	tex->Initialize();
	back->Initialize();
	back->ResetTex(L"img/Blueback.png");
	back->SetScale(XMFLOAT3(300, 300, 300));
	back->SetPos(XMFLOAT3(0, 0, 500));
	manager->SetTowerEnemy(tower);
	//back->Initialize();
	//back->ResetTex(L"img/Blueback.png");
	//back->SetScale(XMFLOAT3(300, 300, 300));
	//back->SetPos(XMFLOAT3(0, 0, 500));
	enemy->SetTower(tower);
	bat->Initialize();
	//enemy->SetTarget(&tower->GetPosition);
	enemy2->state = move1;
	enemy2->SetTower(tower);
	sound->LoadFile(L".\\Resources\\TDBGM2.mp3");
	input->Initialize();
}

void GamePlay::CollisionUpdate()
{
	if (collider->CircleToCircle(*bat->col, *enemy->col))
	{
		if (bat->targetPos == nullptr)
		{
			bat->SetTarget(&enemy->col->position);

			bat->Shot();
		}

	}
	if (bat->bulletList.size() != 0)
	{
		for (int a = 0; a < bat->bulletList.size(); a++)
		{
			if (collider->CircleToCircle(*bat->bulletList[a]->col, *enemy->col))
			{
				enemy->EnemyDamege(bat->damage);
				delete(bat->bulletList[a]);
				bat->bulletList.erase(bat->bulletList.begin() + a);

			}
		}
	}
}
