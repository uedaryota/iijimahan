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
	hp = 100;
}

void Tower::Update()
{

	//Input* input = new Input();
	//input->Initialize();
	//input->Update();
	//float speed = 0.3;
	//if (input->PushKey(DIK_UP))
	//{
	//	//カメラ移動
	//	float len = sqrtf(obj->position.x - Camera::ReturnCameraState()->cameraPos.x + obj->position.y - Camera::ReturnCameraState()->cameraPos.y + obj->position.z - Camera::ReturnCameraState()->cameraPos.z);
	//	XMFLOAT3 vec = XMFLOAT3(Camera::ReturnCameraState()->cameraPos.x / len * speed, Camera::ReturnCameraState()->cameraPos.y / len * speed, Camera::ReturnCameraState()->cameraPos.z / len * speed);
	//	Camera::ReturnCameraState()->cameraPos = XMFLOAT3(Camera::ReturnCameraState()->cameraPos.x - vec.x, Camera::ReturnCameraState()->cameraPos.y - vec.y, Camera::ReturnCameraState()->cameraPos.z - vec.z);

	//}
	//if (input->PushKey(DIK_DOWN))
	//{
	//	//カメラ移動
	//	float len = sqrtf(obj->position.x - Camera::ReturnCameraState()->cameraPos.x + obj->position.y - Camera::ReturnCameraState()->cameraPos.y + obj->position.z - Camera::ReturnCameraState()->cameraPos.z);
	//	XMFLOAT3 vec = XMFLOAT3(Camera::ReturnCameraState()->cameraPos.x / len * speed, Camera::ReturnCameraState()->cameraPos.y / len * speed, Camera::ReturnCameraState()->cameraPos.z / len * speed);
	//	Camera::ReturnCameraState()->cameraPos = XMFLOAT3(Camera::ReturnCameraState()->cameraPos.x + vec.x, Camera::ReturnCameraState()->cameraPos.y + vec.y, Camera::ReturnCameraState()->cameraPos.z + vec.z);

	//}
	//if (input->PushKey(DIK_LEFT))
	//{
	//	//カメラアングル変更
	//	Camera::ReturnCameraState()->eyeangleY += 0.03;
	//}
	//if (input->PushKey(DIK_RIGHT))
	//{
	//	//カメラアングル変更
	//	Camera::ReturnCameraState()->eyeangleY -= 0.03;
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
	XMFLOAT3* position;
	*position = obj->position;
	return* position;
}

void Tower::SetPoisition(XMFLOAT3 position)
{
	obj->position = position;
}

float Tower::GetHp()
{
	return hp;
}

void Tower::SetHp(float x)
{
	hp = hp - x;
}
