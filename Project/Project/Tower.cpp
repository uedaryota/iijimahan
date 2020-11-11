#include "Tower.h"
#include"Input.h"
Tower::Tower()
{

}

Tower::~Tower()
{
	delete(obj);
}

void Tower::Initialize(ID3D12Device* dev)
{
	obj->Initialize(dev);
	obj->LoadObj("triangle_mat");
	obj->position.y = 110;
}

void Tower::Update()
{

	//Input* input = new Input();
	//input->Initialize();
	//input->Update();
	//float speed = 0.3;
	//if (input->PushKey(DIK_UP))
	//{
	//	//�J�����ړ�
	//	float len = sqrtf(obj->position.x - Camera::cameraPos.x + obj->position.y - Camera::cameraPos.y + obj->position.z - Camera::cameraPos.z);
	//	XMFLOAT3 vec = XMFLOAT3(Camera::cameraPos.x / len * speed, Camera::cameraPos.y / len * speed, Camera::cameraPos.z / len * speed);
	//	Camera::cameraPos = XMFLOAT3(Camera::cameraPos.x - vec.x, Camera::cameraPos.y - vec.y, Camera::cameraPos.z - vec.z);

	//}
	//if (input->PushKey(DIK_DOWN))
	//{
	//	//�J�����ړ�
	//	float len = sqrtf(obj->position.x - Camera::cameraPos.x + obj->position.y - Camera::cameraPos.y + obj->position.z - Camera::cameraPos.z);
	//	XMFLOAT3 vec = XMFLOAT3(Camera::cameraPos.x / len * speed, Camera::cameraPos.y / len * speed, Camera::cameraPos.z / len * speed);
	//	Camera::cameraPos = XMFLOAT3(Camera::cameraPos.x + vec.x, Camera::cameraPos.y + vec.y, Camera::cameraPos.z + vec.z);

	//}
	//if (input->PushKey(DIK_LEFT))
	//{
	//	//�J�����A���O���ύX
	//	Camera::eyeangleY += 0.03;
	//}
	//if (input->PushKey(DIK_RIGHT))
	//{
	//	//�J�����A���O���ύX
	//	Camera::eyeangleY -= 0.03;
	//}
	//if (input->PushKey(DIK_SPACE))
	//{

	//}
	obj->Update();
}

void Tower::Draw(ID3D12GraphicsCommandList * cmdList)
{
	obj->Draw(cmdList);
}


XMFLOAT3 Tower::GetPosition()
{
	XMFLOAT3 position;
	position = obj->position;
	return position;
}
