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
#include"EnemyCSVRoder.h"
#include"Cost.h"
#include"ParticleManager.h"
#include"Button.h"
Camera* c = new Camera();
Tower* tower = new Tower();
XMFLOAT3 pointA = { 0,0,30 };
XMFLOAT3 pointB = { 120,0,170 };
XMFLOAT3 SpawnPoint = { -170, 0, -150 };
float HpKari = 1;
float SpeedKari = 1;
//Stage* stage = new Stage();
Stage2D* stage = new Stage2D();

EnemyManeger* manager = new EnemyManeger();
Input* input = new Input();
//vector<Battery*> batL;
vector<DefenderSpawn*> defList;
ObjFile* backSphere;
Sprite* over;
Sprite* clear;
Collision* collider = new Collision();
Spawn* spawn;
Cost* cost = new Cost();
Button* button;
float timer = 0;
float spawntime = 10;
int EneMax = 99;
int Wave = 3;
int EneNow = 0;
int Wave1 = 0;
int Wave2 = 0;
int Wave3 = 0;
int Wave4 = 0;
int Wave2At = 0;
int Wave2Hp = 0;
int Wave2Sp = 0;
int Wave3At = 0;
int Wave3Hp = 0;
int Wave3Sp = 0;
int Wave4At = 0;
int Wave4Hp = 0;
int Wave4Sp = 0;
int BossAt = 0;
int BossHp = 0;
int BossSp = 0;


void GamePlay::Update()
{

	Camera::ReturnCameraState()->cameraPos = XMFLOAT3(0, 135, -255);
	Camera::ReturnCameraState()->target = XMFLOAT3(0, 0, 0);
	Camera::ReturnCameraState()->up = XMFLOAT3(0, 1, 0);

	input->Update();

	if (input->PushKey(DIK_SPACE) && (overFlag || clearFlag))
	{
		endFlag = true;
	}
	if (input->PushKey(DIK_A) )
	{
		clearFlag = true;
	}
	Camera::Update();
	//tower->Update();
	//spawn->Update();
	if (input->TriggerKey(DIK_B))
	{
		tower->Damage(10);
	}
	//
	//
	//	sound->Update();`

	
	//クリアフラグ、ゲームオーバーフラグ共にFalseでUpdate
	if (!clearFlag && !overFlag)
	{
		tower->Update();
		spawn->Update();
		//	sound->Update();

		switch (NowWAVE)
		{
		case wave1:
			if (EneNow < Wave1)
			{
				timer++;
				if (timer / 60 > spawntime)
				{
					//エネミー生成
					manager->Add2(spawn->GetPosition());
					manager->ReAncerSet(pointA, pointB);
					manager->SetTowerEnemy(tower);
					manager->SetHp(HpKari);
					manager->SetSpeed(SpeedKari);
					timer = 0;
					EneNow++;
				}
			}
			else
			{
				if (EneNow == manager->CountDeath())
				{
					timer = -(60 * spawntime / 2);
					NowWAVE = wave2;
				}
			}
			break;
		case wave2:
			if (EneNow < Wave1 + Wave2)
			{
				timer++;
				if (timer / 60 > spawntime)
				{
					//エネミー生成
					manager->Add2(spawn->GetPosition());
					manager->ReAncerSet(pointA, pointB);
					manager->SetTowerEnemy(tower);
					manager->SetPower(Wave2At);
					manager->SetHp(Wave2Hp);
					manager->SetSpeed(Wave2Sp);
					timer = 0;
					EneNow++;
				}
			}
			else
			{
				if (EneNow == manager->CountDeath() - 1)
				{
					timer = -(60 * spawntime / 2);
					NowWAVE = wave3;
				}
			}
			break;
		case wave3:
			if (EneNow < Wave1 + Wave2 + Wave3+1)
			{
				timer++;
				if (timer / 60 > spawntime)
				{
					//エネミー生成
					manager->Add2(spawn->GetPosition());
					manager->ReAncerSet(pointA, pointB);
					manager->SetTowerEnemy(tower);
					manager->SetPower(Wave3At);
					manager->SetHp(Wave3Hp);
					manager->SetSpeed(Wave3Sp);
					timer = 0;
					EneNow++;
				}
			}
			else
			{
				if (EneNow == manager->CountDeath() - 1)
				{
					NowWAVE = wave4;
				}
			}
			break;
		case wave4:
			if (EneNow < Wave1 + Wave2 + Wave3 + Wave4+1)
			{
				timer++;
				if (timer / 60 > spawntime)
				{
					//エネミー生成
					manager->Add2(spawn->GetPosition());
					manager->ReAncerSet(pointA, pointB);
					manager->SetTowerEnemy(tower);
					manager->SetPower(Wave4At);
					manager->SetHp(Wave4Hp);
					manager->SetSpeed(Wave4Sp);
					timer = 0;
					EneNow++;
				}
			}
			else if (EneNow < Wave1 + Wave2 + Wave3 + Wave4 +2)
			{
				timer++;
				if (timer / 60 > spawntime)
				{
					//エネミー生成
					manager->Add2(spawn->GetPosition());
					manager->BossCreate();
					manager->ReAncerSet(pointA, pointB);
					manager->SetTowerEnemy(tower);
					manager->BossHp(BossHp);
					manager->BossPower(BossAt);
					manager->BossSpeed(BossSp);
					timer = 0;
					EneNow++;
				}
			}
			else
			{
				if (EneNow == manager->CountDeath() - 1)
				{
					NowWAVE = wave0;
				}
			}
			break;
		case wave0:
			break;
		}
		manager->Update();

		stage->Update();
		for (int a = 0; a < defList.size(); a++)
		{
			defList[a]->Update();
		}
		if (cost->cost >= 20)
		{
			button->costflag = true;
		}
		else
		{
			button->costflag = false;
		}

		cost->Update();
		backSphere->Update();
		//backSphere->SetPos({ 500,500,500 });
		
		CostUpdate();
		button->Update();
		CollisionUpdate();
		EndFlagCheck();
	}

	sound->ChkRoop();
}
GamePlay::GamePlay()
{
}
void GamePlay::Draw()
{

	manager->Draw(DirectXDevice::cmdList);
	spawn->Draw(DirectXDevice::cmdList);
	stage->Draw();
	tower->Draw(DirectXDevice::cmdList);
	for (int a = 0; a < defList.size(); a++)
	{
		defList[a]->Draw();
	}
	cost->Draw();
	button->Draw();
	backSphere->Draw(DirectXDevice::cmdList);

	Sprite::PreDraw(DirectXDevice::cmdList);
	ClickUI->Draw();
	ArrowkeyUI->Draw();
	text->Print("NEXT  " + to_string(int(spawntime - timer / 60)), 30, 250, 3);
	nextback->Draw();
	text->DrawAll(DirectXDevice::cmdList);
	Sprite::PostDraw();

	if (clearFlag)
	{
		Sprite::PreDraw(DirectXDevice::cmdList);
		clear->Draw();
		//sprite2->Draw();
		Sprite::PostDraw();
	}
	if (overFlag)
	{
		Sprite::PreDraw(DirectXDevice::cmdList);
		over->Draw();
		//sprite2->Draw();
		Sprite::PostDraw();
	}

	Sprite::PreDraw(DirectXDevice::cmdList);
	text->Print(to_string(NowWAVE + 1)+ "Next  " + to_string(int(spawntime - timer / 60)), 30, 250, 3);
	nextback->Draw();
	text->DrawAll(DirectXDevice::cmdList);
	Sprite::PostDraw();
}
void GamePlay::Initialize()
{
	overFlag = false;
	clearFlag = false;
	endFlag = false;
	
	Sprite::LoadTexture(3, L"img/GameClear.png");
	Sprite::LoadTexture(4, L"img/GameOver.png");
	Sprite::LoadTexture(6, L"img/costback.png");
	Sprite::LoadTexture(8, L"img/ClickUI.png");
	Sprite::LoadTexture(9, L"img/ArrowkeyUI.png");

	//クリア画像読込,読込済みの3番
	clear = Sprite::Create(3, { 0.0f, 0.0f });
	clear->SetSize({ static_cast<float>(Camera::window_width),static_cast<float>(Camera::window_height) });
//	clear->SetPosition(XMFLOAT2{ static_cast<float>(Camera::window_width) / 2, static_cast<float>(Camera::window_height) / 2 });
//	clear->SetSize(XMFLOAT2{ 1 * (static_cast<float>(Camera::window_width) / static_cast<float>(Camera::window_height)),1 });
	//ゲームオーバー画像読込,込済みの4番
	over = Sprite::Create(4, { 0.0f, 0.0f });

	over->SetSize({ static_cast<float>(Camera::window_width),static_cast<float>(Camera::window_height) });
//	over->SetPosition(XMFLOAT2{ static_cast<float>(Camera::window_width) / 2, static_cast<float>(Camera::window_height) / 2 });
//	over->SetSize(XMFLOAT2{ 1 * (static_cast<float>(Camera::window_width) / static_cast<float>(Camera::window_height)),1 });
	nextback = Sprite::Create(6, {0.0f,10.0f});

	ClickUI = Sprite::Create(8, { -10.0f, -50.0f });
	ClickUI->SetSize(XMFLOAT2(300, 300));

	ArrowkeyUI = Sprite::Create(9, { 0.0f, 20.0f });
	ArrowkeyUI->SetSize(XMFLOAT2(300, 300));

	tower->Initialize(DirectXDevice::dev);
	tower->SetPoisition({ 130,0,180 });
	manager->Initialize();
	sound->Initialize();
	stage->Initialize();
	spawn = new Spawn();
	spawn->Initialize(DirectXDevice::dev);
	//Set = &SetAd;
	spawn->SetSpawn(10, 10);
	spawn->SetPoisition(SpawnPoint);
	manager->Add2(spawn->GetPosition());
	timer = 0;
	manager->SetTowerEnemy(tower);
	
	DefenderSpawn* d1 = new DefenderSpawn();
	d1->Initialize();
	d1->SetPos({ -40, 0, -90 });
	defList.push_back(d1);

	DefenderSpawn* d2 = new DefenderSpawn();
	d2->Initialize();
	d2->SetPos({ -70, 0, -190 });
	defList.push_back(d2);

	DefenderSpawn* d3 = new DefenderSpawn();
	d3->Initialize();
	d3->SetPos({ 50, 0, -110 });
	defList.push_back(d3);

	DefenderSpawn* d4 = new DefenderSpawn();
	d4->Initialize();
	d4->SetPos({ -20, 0, 30 });
	defList.push_back(d4);

	DefenderSpawn* d5 = new DefenderSpawn();
	d5->Initialize();
	d5->SetPos({ 70, 0, 60 });
	defList.push_back(d5);

	DefenderSpawn* d6 = new DefenderSpawn();
	d6->Initialize();
	d6->SetPos({ 170, 0, 110 });
	defList.push_back(d6);

	DefenderSpawn* d7 = new DefenderSpawn();
	d7->Initialize();
	d7->SetPos({ 170, 0, 25 });
	defList.push_back(d7);

	sound->LoadFile(L".\\Resources\\TDBGM2.mp3");
	sound->LoadFile(L".\\Resources\\TDBGM3.mp3");
	input->Initialize();
	vector<DATA>DateTable = LoadData("Data/Enemy.csv");
	for (auto date : DateTable)
	{
		pointA = date.FirstPos;
		pointB = date.SecondPos;
		HpKari = date.HP;
		SpeedKari = date.SPEED;
		SpawnPoint = date.SpawnSpot;	
		Wave1 = date.Wave1;
		Wave2 = date.Wave2;
		Wave3 = date.Wave3;
		Wave4 = date.Wave4;
		Wave2At = date.Wave2Attack;
		Wave2Hp = date.Wave2Hp;
		Wave2Sp = date.Wave2Speed;
		Wave3At = date.Wave3Attack;
		Wave3Hp = date.Wave3Hp;
		Wave3Sp = date.Wave3Speed;
		Wave4At = date.Wave4Attack;
		Wave4Hp = date.Wave4Hp;
		Wave4Sp = date.Wave4Speed;
		BossAt = date.BossAttack;
		BossHp = date.BossHp;
		BossSp = date.BossSpeed;
	}
	//背景
	backSphere = new ObjFile();
	backSphere->Initialize();
	backSphere->LoadObj("BackSphere");
	backSphere->scale = { 1000, 1000, 1000 };
	manager->SetSpeed(SpeedKari);
	NowWAVE=wave1;

	cost = new Cost();
	cost->Initialize();

	button = new Button();
	button->Initialize();

	sound->PlayRoop();
	//テキスト初期化
	text = Text::GetInstance();
	text->Initialize(0);
	nextback = Sprite::Create(5, { 0.0f,250.0f });
	nextback->SetSize(XMFLOAT2(350, 50));
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
	}
	for (int i = 0; i < defList.size(); i++)
	{
		for (int a = 0; a < manager->boxcount; a++)
		{
			bool AllDead = true;
			if (defList[i]->battery->liveFlag&&manager->enemybox[a] != nullptr && manager->enemybox[a]->Deadflag==false)
			{
				AllDead = false;
				if (collider->CircleToCircle(defList[i]->battery->col, manager->enemybox[a]->col) && manager->enemybox[a]->Hp >= 0)
				{
					targetFlag = true;
					if (defList[i]->battery->targetPos == nullptr)
					{
						//砲台にターゲットをセット
						defList[i]->battery->SetTarget(&manager->enemybox[a]->col->position);
					}
					if (manager->enemybox[a]->Hp <= 0)
					{
						//HP0になったらターゲットnull
						defList[i]->battery->SetTarget(nullptr);
					}
				}
				else if (!targetFlag)
				{
					//範囲外ならnull
					defList[i]->battery->SetTarget(nullptr);
				}
				if (defList[i]->battery->bulletList.size() != 0)
				{
					for (int b = 0; b < defList[i]->battery->bulletList.size(); b++)
					{
						if (collider->CircleToCircle(defList[i]->battery->bulletList[b]->col, manager->enemybox[a]->col)
							&& defList[i]->battery->bulletList[b]->liveFlag)
						{
							manager->enemybox[a]->EnemyDamege(defList[i]->battery->damage);
							defList[i]->battery->bulletList[b]->Reset();
						}
					}
				}
				if (!targetFlag)
				{
				//	defList[i]->battery->SetTarget(nullptr);
				}
			}
	//		if (AllDead)
			{
				//return;
			}
		}
	}
}

void GamePlay::CostUpdate()
{
	for (int a = 0; a < defList.size(); a++)
	{
		//コストフラグがTrueなら
		if (defList[a]->ReturnCostFlag())
		{
			//現在コストが使用コスト以上ならば
			if (cost->GetCost() >= defList[a]->ReturnUseCost())
			{
				cost->SetSubCost(defList[a]->usecost);
				defList[a]->CostFlagFalse();
			}
			//それ以外
			else
			{
				defList[a]->StopCreate();
			}
		}	
	}
}

void GamePlay::EndFlagCheck()
{
	if (tower->GetHp() <= 0)
	{
		overFlag = true;
	}
	if (manager->boxcount <= 0)
	{
		clearFlag = true;
	}
}
