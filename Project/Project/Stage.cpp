#include "Stage.h"
#include"DirectXDevice.h"
Stage::Stage()
{

}

void Stage::Initialize()
{
	CreateStage(allFloor);
	input = new Input();
	input->Initialize();
}
void Stage::CreateStage(int count)
{
	for (int i = 0; i < count; i++)
	{
		Floor* floor1 = new Floor();
		Floor* wall1 = new Floor();
		Floor* wall2 = new Floor();
		Floor* wall3 = new Floor();
		Floor* wall4 = new Floor();

		stageList.emplace_back(floor1);
		stageList.emplace_back(wall1);
		stageList.emplace_back(wall2);
		stageList.emplace_back(wall3);
		stageList.emplace_back(wall4);

		for (int a = 0; a < 5; a++)
		{
			stageList[a + i * 5]->Initialize(DirectXDevice::dev);
		}
	
		for (int a = 0; a < 4; a++)
		{
			stageList[1 + a + i * 5]->CreateWallPipeline(DirectXDevice::dev);
			stageList[1 + a + i * 5]->ResetTex(L"img/tex1.png", DirectXDevice::dev);
			stageList[1 + a + i * 5]->rotation = XMFLOAT3(0, a*XM_PIDIV2, 0);
			stageList[1 + a + i * 5]->scale = XMFLOAT3(100, 50, 100);

		}
		int height = 100;//�ǂ̍���
		stageList[0 + i * 5]->position = XMFLOAT3(0, i *height, 0);
		stageList[1 + i * 5]->position = XMFLOAT3(0, 50 + i * height, 100);
		stageList[2 + i * 5]->position = XMFLOAT3(100, 50 + i * height, 0);
		stageList[3 + i * 5]->position = XMFLOAT3(0, 50 + i * height, -100);
		stageList[4 + i * 5]->position = XMFLOAT3(-100, 50 + i * height, 0);

		stageList[i * 5]->constalpha = 0.5f;
		stageList[0]->constalpha = 1.0f;

	}
}
void Stage::Update()
{
	
	input->Update();

	if (input->PushKey(DIK_UP) && floorCount < allFloor && !push || input->PushKey(DIK_W) && floorCount < allFloor && !push)
	{
		//�J�����ړ�
		push = true;
		floorCount++;
		Camera::SetTarget(stageList[(floorCount - 1) * 5]->position);
		for (int i = 0; i < allFloor; i++)
		{
			stageList[i * 5]->constalpha = 0.5f;
		}
		stageList[(floorCount - 1) * 5]->constalpha = 1.0f;

		Camera::ReturnCameraState()->cameraPos.y += 100;
		/*float len = sqrtf(floor1_1->position.x - Camera::ReturnCameraState()->cameraPos.x + floor1_1->position.y - Camera::ReturnCameraState()->cameraPos.y + floor1_1->position.z - Camera::ReturnCameraState()->cameraPos.z);
		XMFLOAT3 vec = XMFLOAT3(Camera::ReturnCameraState()->cameraPos.x / len * speed, Camera::ReturnCameraState()->cameraPos.y / len * speed, Camera::ReturnCameraState()->cameraPos.z / len * speed);
		Camera::ReturnCameraState()->cameraPos = XMFLOAT3(Camera::ReturnCameraState()->cameraPos.x - vec.x, Camera::ReturnCameraState()->cameraPos.y - vec.y, Camera::ReturnCameraState()->cameraPos.z - vec.z);*/

	}
	

	else if (input->PushKey(DIK_DOWN) && floorCount > 1 && !push || input->PushKey(DIK_S) && floorCount > 1 && !push)
	{

		push = true;
		floorCount--;
		Camera::SetTarget(stageList[(floorCount - 1) * 5]->position);
		for (int i = 0; i < allFloor; ++i)
		{
			stageList[i * 5]->constalpha = 0.5f;
		}
		stageList[(floorCount - 1) * 5]->constalpha = 1.0f;

		Camera::ReturnCameraState()->cameraPos.y -= 100;
		//�J�����ړ�
		/*float len = sqrtf(floor1_1->position.x - Camera::ReturnCameraState()->cameraPos.x + floor1_1->position.y - Camera::ReturnCameraState()->cameraPos.y + floor1_1->position.z - Camera::ReturnCameraState()->cameraPos.z);
		XMFLOAT3 vec = XMFLOAT3(Camera::ReturnCameraState()->cameraPos.x / len * speed, Camera::ReturnCameraState()->cameraPos.y / len * speed, Camera::ReturnCameraState()->cameraPos.z / len * speed);
		Camera::ReturnCameraState()->cameraPos = XMFLOAT3(Camera::ReturnCameraState()->cameraPos.x + vec.x, Camera::ReturnCameraState()->cameraPos.y + vec.y, Camera::ReturnCameraState()->cameraPos.z + vec.z);*/

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
		//�J�����A���O���ύX
		Camera::ReturnCameraState()->eyeangleY += 0.03;
	}
	if (input->PushKey(DIK_RIGHT)|| input->PushKey(DIK_D))
	{
		//�J�����A���O���ύX
		Camera::ReturnCameraState()->eyeangleY -= 0.03;
	}
	if (input->PushKey(DIK_SPACE))
	{

	}
	for (int i = 0; i < stageList.size(); i++)
	{
		stageList[i]->Update();
	}
}

void Stage::Draw()
{
	for (int i = 0; i < stageList.size(); i++)
	{
		stageList[i]->Draw(DirectXDevice::cmdList, DirectXDevice::dev);
	}
}
