#include "Stage.h"
#include"DirectXDevice.h"
Stage::Stage()
{

}

void Stage::Initialize()
{
	floor1_1->Initialize(DirectXDevice::dev);
	floor1_2->Initialize(DirectXDevice::dev);
	floor1_3->Initialize(DirectXDevice::dev);

	floor1_2->ResetTex(L"img/tex1.png", DirectXDevice::dev);
	floor1_3->ResetTex(L"img/tex1.png", DirectXDevice::dev);

	floor1_2->rotation = XMFLOAT3(0, XM_PIDIV2, 0);
	floor1_3->rotation = XMFLOAT3(0, 0, 0);

	floor1_2->scale = XMFLOAT3(100, 50, 100);
	floor1_3->scale = XMFLOAT3(100, 50, 100);

	floor1_2->position = XMFLOAT3(100, 50, 0);
	floor1_3->position = XMFLOAT3(0, 50, 100);

	floor2_1->Initialize(DirectXDevice::dev);
	floor2_2->Initialize(DirectXDevice::dev);
	floor2_3->Initialize(DirectXDevice::dev);

	floor2_2->ResetTex(L"img/tex1.png", DirectXDevice::dev);
	floor2_3->ResetTex(L"img/tex1.png", DirectXDevice::dev);

	floor2_2->rotation = XMFLOAT3(0, XM_PIDIV2, 0);
	floor2_3->rotation = XMFLOAT3(0, 0, 0);

	floor2_2->scale = XMFLOAT3(100, 50, 100);
	floor2_3->scale = XMFLOAT3(100, 50, 100);

	floor2_1->position = XMFLOAT3(0, 100, 0);
	floor2_2->position = XMFLOAT3(100, 150, 0);
	floor2_3->position = XMFLOAT3(0, 150, 100);

	floor3_1->Initialize(DirectXDevice::dev);
	floor3_2->Initialize(DirectXDevice::dev);
	floor3_3->Initialize(DirectXDevice::dev);

	floor3_2->ResetTex(L"img/tex1.png", DirectXDevice::dev);
	floor3_3->ResetTex(L"img/tex1.png", DirectXDevice::dev);

	floor3_2->rotation = XMFLOAT3(0, XM_PIDIV2, 0);
	floor3_3->rotation = XMFLOAT3(0, 0, 0);

	floor3_2->scale = XMFLOAT3(100, 50, 100);
	floor3_3->scale = XMFLOAT3(100, 50, 100);

	floor3_1->position = XMFLOAT3(0, 200, 0);
	floor3_2->position = XMFLOAT3(100, 250, 0);
	floor3_3->position = XMFLOAT3(0, 250, 100);

	floor1_1->constalpha = 1.0f;
	floor2_1->constalpha = 0.5f;
	floor3_1->constalpha = 0.5f;

}
void Stage::Update()
{
	Input* input = new Input();
	input->Initialize();
	input->Update();

	if (input->PushKey(DIK_UP) || input->PushKey(DIK_W) && floorCount == 1 && !push)
	{
		//カメラ移動
		push = true;
		floorCount++;
		Camera::SetTarget(floor2_1->position);
		floor1_1->constalpha = 0.5f;
		floor2_1->constalpha = 1.0f;
		floor3_1->constalpha = 0.5f;

		Camera::cameraPos.y += 100;
		/*float len = sqrtf(floor1_1->position.x - Camera::cameraPos.x + floor1_1->position.y - Camera::cameraPos.y + floor1_1->position.z - Camera::cameraPos.z);
		XMFLOAT3 vec = XMFLOAT3(Camera::cameraPos.x / len * speed, Camera::cameraPos.y / len * speed, Camera::cameraPos.z / len * speed);
		Camera::cameraPos = XMFLOAT3(Camera::cameraPos.x - vec.x, Camera::cameraPos.y - vec.y, Camera::cameraPos.z - vec.z);*/

	}
	else if (input->PushKey(DIK_UP) || input->PushKey(DIK_W) && floorCount == 2 && !push)
	{
		//カメラ移動
		push = true;
		floorCount++;
		Camera::SetTarget(floor3_1->position);
		floor1_1->constalpha = 0.5f;
		floor2_1->constalpha = 0.5f;
		floor3_1->constalpha = 1.0f;

		Camera::cameraPos.y += 100;
		/*float len = sqrtf(floor1_1->position.x - Camera::cameraPos.x + floor1_1->position.y - Camera::cameraPos.y + floor1_1->position.z - Camera::cameraPos.z);
		XMFLOAT3 vec = XMFLOAT3(Camera::cameraPos.x / len * speed, Camera::cameraPos.y / len * speed, Camera::cameraPos.z / len * speed);
		Camera::cameraPos = XMFLOAT3(Camera::cameraPos.x - vec.x, Camera::cameraPos.y - vec.y, Camera::cameraPos.z - vec.z);*/

	}

	else if (input->PushKey(DIK_DOWN) || input->PushKey(DIK_S) && floorCount == 2 && !push)
	{

		push = true;
		floorCount--;
		Camera::SetTarget(floor1_1->position);
		floor1_1->constalpha = 1.0f;
		floor2_1->constalpha = 0.5f;
		floor3_1->constalpha = 0.5f;

		Camera::cameraPos.y -= 100;
		//カメラ移動
		/*float len = sqrtf(floor1_1->position.x - Camera::cameraPos.x + floor1_1->position.y - Camera::cameraPos.y + floor1_1->position.z - Camera::cameraPos.z);
		XMFLOAT3 vec = XMFLOAT3(Camera::cameraPos.x / len * speed, Camera::cameraPos.y / len * speed, Camera::cameraPos.z / len * speed);
		Camera::cameraPos = XMFLOAT3(Camera::cameraPos.x + vec.x, Camera::cameraPos.y + vec.y, Camera::cameraPos.z + vec.z);*/

	}
	else if(input->PushKey(DIK_DOWN) || input->PushKey(DIK_S) && floorCount == 3 && !push)
	{

		push = true;
		floorCount--;
		Camera::SetTarget(floor2_1->position);
		floor1_1->constalpha = 0.5f;
		floor2_1->constalpha = 1.0f;
		floor3_1->constalpha = 0.5f;

		Camera::cameraPos.y -= 100;
		//カメラ移動
		/*float len = sqrtf(floor1_1->position.x - Camera::cameraPos.x + floor1_1->position.y - Camera::cameraPos.y + floor1_1->position.z - Camera::cameraPos.z);
		XMFLOAT3 vec = XMFLOAT3(Camera::cameraPos.x / len * speed, Camera::cameraPos.y / len * speed, Camera::cameraPos.z / len * speed);
		Camera::cameraPos = XMFLOAT3(Camera::cameraPos.x + vec.x, Camera::cameraPos.y + vec.y, Camera::cameraPos.z + vec.z);*/

	}

	else if (push&&input->PushKey(DIK_DOWN) || input->PushKey(DIK_UP) || input->PushKey(DIK_W) || input->PushKey(DIK_S))
	{
		push = true;
	}
	else
	{
		push = false;
	}


	if (input->PushKey(DIK_LEFT)||input->PushKey(DIK_A))
	{
		//カメラアングル変更
		Camera::eyeangleY += 0.03;
	}
	if (input->PushKey(DIK_RIGHT)|| input->PushKey(DIK_D))
	{
		//カメラアングル変更
		Camera::eyeangleY -= 0.03;
	}
	if (input->PushKey(DIK_SPACE))
	{

	}
	floor1_1->Update();
	floor1_2->Update();
	floor1_3->Update();

	floor2_1->Update();
	floor2_2->Update();
	floor2_3->Update();

	floor3_1->Update();
	floor3_2->Update();
	floor3_3->Update();

}

void Stage::Draw()
{
	floor1_1->Draw(DirectXDevice::cmdList, DirectXDevice::dev);
	floor1_2->Draw(DirectXDevice::cmdList, DirectXDevice::dev);
	floor1_3->Draw(DirectXDevice::cmdList, DirectXDevice::dev);

	floor2_1->Draw(DirectXDevice::cmdList, DirectXDevice::dev);
	floor2_2->Draw(DirectXDevice::cmdList, DirectXDevice::dev);
	floor2_3->Draw(DirectXDevice::cmdList, DirectXDevice::dev);

	floor3_1->Draw(DirectXDevice::cmdList, DirectXDevice::dev);
	floor3_2->Draw(DirectXDevice::cmdList, DirectXDevice::dev);
	floor3_3->Draw(DirectXDevice::cmdList, DirectXDevice::dev);

}
