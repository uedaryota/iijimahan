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

Camera* c = new Camera();
Tower* tower = new Tower();
XMFLOAT3 pointA = { 0,0,30 };
XMFLOAT3 pointB = { 120,0,170 };
float HpKari = 1;
float SpeedKari = 1;
Sound* sound = new Sound();
//Stage* stage = new Stage();
Stage2D* stage = new Stage2D();
Enemy* enemy = new Enemy();
Enemy* enemy2 = new Enemy();
EnemyAI* ai = new EnemyAI();
EnemyManeger* manager = new EnemyManeger();
Input* input = new Input();
//vector<Battery*> batL;
vector<DefenderSpawn*> defList;
ObjFile* backSphere;
Sprite* over;
Sprite* clear;
Collision* collider = new Collision();
Spawn* spawn = new Spawn();
Cost* cost = new Cost();

float timer = 0;
float spawntime = 10;
int EneMax = 30;
int Wave = 3;
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
	if (input->PushKey(DIK_SPACE) && (overFlag || clearFlag))
	{
		endFlag = true;
	}
	Camera::Update();
	//�N���A�t���O�A�Q�[���I�[�o�[�t���O����False��Update
	if (!clearFlag && !overFlag)
	{
		tower->Update();
		spawn->Update();
		//	sound->Update();
		if (EneNow < EneMax)
		{
			timer++;
			if (timer / 60 > spawntime)
			{
				//�G�l�~�[����
				manager->Add2(spawn->GetPosition());
				manager->ReAncerSet(pointA, pointB);
				manager->SetTowerEnemy(tower);
				manager->SetHp(HpKari);
				manager->SetSpeed(SpeedKari);
				timer = 0;
				EneNow++;
			}
		}


		//enemy->SetScale(XMFLOAT3{ 10,10,10 });
		manager->Update();

		stage->Update();
		backSphere->Update();
		for (int a = 0; a < defList.size(); a++)
		{
			defList[a]->Update();
		}

		//if (input->PushKey(DIK_Q))//�����p���������ʐ����@�����W�̕ύX���s���܂��B
		//{
		//	manager->AncerSet(XMFLOAT3{ 15,15,15 }, XMFLOAT3{ 15,15,15 });
		//}
		//if (input->PushKey(DIK_SPACE))//�����p���������ʐ����@�����W�̕ύX���s���܂��B
		//{
		//	manager->ReAncerSet(XMFLOAT3{ -100,1,-100 }, XMFLOAT3{ 500,500,500 });
		//}
		//if (input->TriggerKey(DIK_Z))
		//{
		//	enemy->EnemyDamege(1);
		//	//enemy2->EnemyDamege(0.5);
		//}
		cost->Update();

		CostUpdate();
		CollisionUpdate();
		EndFlagCheck();
	}


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
	backSphere->Draw(DirectXDevice::cmdList);
	cost->Draw();
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

}
void GamePlay::Initialize()
{
	overFlag = false;
	clearFlag = false;
	endFlag = false;

	//Sprite::LoadTexture(1, L"img/TowerDefence_Title.png");
	//Sprite::LoadTexture(2, L"img/TowerDefence_TitleBack.png");
	Sprite::LoadTexture(3, L"img/title.png");
	Sprite::LoadTexture(4, L"img/end.png");

	//�N���A�摜�Ǎ�,�Ǎ��ς݂�3��
	clear = Sprite::Create(3, { 0.0f, 0.0f });
//	clear->SetPosition(XMFLOAT2{ static_cast<float>(Camera::window_width) / 2, static_cast<float>(Camera::window_height) / 2 });
//	clear->SetSize(XMFLOAT2{ 1 * (static_cast<float>(Camera::window_width) / static_cast<float>(Camera::window_height)),1 });
	//�Q�[���I�[�o�[�摜�Ǎ�,���ς݂�4��
	over = Sprite::Create(4, { 0.0f, 0.0f });
//	over->SetPosition(XMFLOAT2{ static_cast<float>(Camera::window_width) / 2, static_cast<float>(Camera::window_height) / 2 });
//	over->SetSize(XMFLOAT2{ 1 * (static_cast<float>(Camera::window_width) / static_cast<float>(Camera::window_height)),1 });

	tower->Initialize(DirectXDevice::dev);
	tower->SetPoisition({ 130,0,180 });
	manager->Initialize();
	enemy->Initialize();
	sound->Initialize();
	stage->Initialize();
	spawn->Initialize(DirectXDevice::dev);
	//Set = &SetAd;
	spawn->SetSpawn(10, 10);
	spawn->SetPoisition({ -170, 0, -150 });
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

	//enemy->SetTarget(&tower->GetPosition);
	//�G
	//enemy2->state = move1;
	//enemy2->SetTower(tower);
	sound->LoadFile(L".\\Resources\\TDBGM2.mp3");
	input->Initialize();
	vector<DATA>DateTable = LoadData("Data/Enemy.csv");
	for (auto date : DateTable)
	{
		pointA = date.FirstPos;
		pointB = date.SecondPos;
		HpKari = date.HP;
		SpeedKari = date.SPEED;
	}
	//�w�i
	backSphere = new ObjFile();
	backSphere->Initialize();
	backSphere->LoadObj("BackSphere");
	backSphere->scale = { 1000, 1000, 1000 };

	cost->Initialize();
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
			if (defList[i]->battery->liveFlag&&manager->enemybox[a] != nullptr)
			{
				if (collider->CircleToCircle(defList[i]->battery->col, manager->enemybox[a]->col) && manager->enemybox[a]->Hp >= 0)
				{
					targetFlag = true;
					if (defList[i]->battery->targetPos == nullptr)
					{
						//�C��Ƀ^�[�Q�b�g���Z�b�g
						defList[i]->battery->SetTarget(&manager->enemybox[a]->col->position);
					}
					if (manager->enemybox[a]->Hp <= 0)
					{
						//HP0�ɂȂ�����^�[�Q�b�gnull
						defList[i]->battery->SetTarget(nullptr);
					}
				}
				else if (!targetFlag)
				{
					//�͈͊O�Ȃ�null
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
		}
	}
}

void GamePlay::CostUpdate()
{
	for (int a = 0; a < defList.size(); a++)
	{
		//�R�X�g�t���O��True�Ȃ�
		if (defList[a]->ReturnCostFlag())
		{
			//���݃R�X�g���g�p�R�X�g�ȏ�Ȃ��
			if (cost->GetCost() >= defList[a]->ReturnUseCost())
			{
				cost->SetSubCost(defList[a]->usecost);
				defList[a]->CostFlagFalse();
			}
			//����ȊO
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
