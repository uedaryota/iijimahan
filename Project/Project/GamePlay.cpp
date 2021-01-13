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
#include"DefenderSpawn.h"
#include<vector>
Camera* c = new Camera();
Tower* tower = new Tower();
XMFLOAT3 pointA = { 0,0,30 };
XMFLOAT3 pointB = { 120,0,170 };
Sound* sound = new Sound();
//Stage* stage = new Stage();
Stage2D* stage = new Stage2D();
Enemy* enemy = new Enemy();
Enemy* enemy2 = new Enemy();
EnemyAI* ai = new EnemyAI();
EnemyManeger* manager = new EnemyManeger();
Input* input = new Input();
//vector<Battery*> defList;
vector<DefenderSpawn*> defList;


Collision* collider = new Collision();
Spawn* spawn = new Spawn();

float timer = 0;
float spawntime = 10;
int EneMax = 3;
int EneNow = 0;
void GamePlay::Update()
{

	Camera::ReturnCameraState()->cameraPos = XMFLOAT3(0, 135, -255);
	Camera::ReturnCameraState()->target = XMFLOAT3(0, 0, 0);
	Camera::ReturnCameraState()->up = XMFLOAT3(0, 1, 0);

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
			manager->ReAncerSet(pointA, pointB);
			manager->SetTowerEnemy(tower);
			timer = 0;
			EneNow++;
		}
	}
	

	enemy->SetScale(XMFLOAT3{ 10,10,10 });
	manager->Update();

	stage->Update();

	for (int a = 0; a < defList.size(); a++)
	{
		defList[a]->Update();
	}

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
	for (int a = 0; a < defList.size(); a++)
	{
		defList[a]->Draw();
	}
}
void GamePlay::Initialize()
{
	tower->Initialize(DirectXDevice::dev);
	manager->Initialize();
	enemy->Initialize();
	sound->Initialize();
	stage->Initialize();
	spawn->Initialize(DirectXDevice::dev);
	//Set = &SetAd;
	spawn->SetSpawn(10, 10);
	spawn->SetPoisition({ -170, 0, -150 });
	manager->Add2(spawn->GetPosition());
	//manager->Add(enemy);
	//manager->Add(enemy2);
	enemy->state = move1;
	timer = 0;
	manager->SetTowerEnemy(tower);
	//back->Initialize();
	//back->ResetTex(L"img/Blueback.png");
	//back->SetScale(XMFLOAT3(300, 300, 300));
	//back->SetPos(XMFLOAT3(0, 0, 500));
	enemy->SetTower(tower);
	DefenderSpawn* d1 = new DefenderSpawn();
	d1->Initialize();
	d1->SetPos({ -40, 0, -90 });
	defList.push_back(d1);
	//enemy->SetTarget(&tower->GetPosition);
	enemy2->state = move1;
	enemy2->SetTower(tower);
	sound->LoadFile(L".\\Resources\\TDBGM2.mp3");
	input->Initialize();
}

void GamePlay::CollisionUpdate()
{
	bool targetFlag = false;
	for (int i = 0; i < defList.size(); i++)
	{
		if (input->PushMouseLeft())
		{
			if (collider->MouseToCircle(input, defList[i]->col))
			{
				defList[i]->CreateBattery();
			}
		}
		for (int a = 0; a < manager->boxcount; a++)
		{
			if (defList[i]->battery != nullptr)
			{


				if (collider->CircleToCircle(*defList[i]->battery->col, *manager->enemybox[a]->col))
				{
					targetFlag = true;
					if (defList[i]->battery->targetPos == nullptr)
					{
						defList[i]->battery->SetTarget(&manager->enemybox[a]->col->position);
						//defList[0]->Shot();
					}
					if (manager->enemybox[a]->Hp <= 0)
					{
						defList[i]->battery->SetTarget(nullptr);
					}
				}


				if (defList[i]->battery->bulletList.size() != 0)
				{
					for (int b = 0; b < defList[i]->battery->bulletList.size(); b++)
					{
						if (collider->CircleToCircle(*defList[i]->battery->bulletList[b]->col, *manager->enemybox[a]->col))
						{
							manager->enemybox[a]->EnemyDamege(defList[i]->battery->damage);
							delete(defList[i]->battery->bulletList[b]);
							defList[i]->battery->bulletList.erase(defList[i]->battery->bulletList.begin() + b);
						}
					}
				}


				if (!targetFlag)
				{
					defList[i]->battery->SetTarget(nullptr);
				}
			}
		}
	}
}
