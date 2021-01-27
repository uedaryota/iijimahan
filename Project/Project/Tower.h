#pragma once
#include"Camera.h"
#include"ObjFile.h"
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include "Sprite.h"
#include "DirectXDevice.h"
#include "Text.h"
#include"ParticleManager.h"
class Tower
{
public:
	Tower();
	~Tower();
	void Initialize(ID3D12Device* dev);
	void Update();
	void Draw(ID3D12GraphicsCommandList * cmdList);
	XMFLOAT3 GetPosition();//11/4�G�l�~�[�p�Ɉʒu�𑗂�܂��Bby���}�i�J
	void SetPoisition(XMFLOAT3 position);//11/11�A���J�[�悤�Ɉʒu�ύX���ł���悤�ɂ��܂��B
	float GetHp();
	void SetHp(float x);
	void Damage(float damage);
	float hp =100.0f;
private:
	float maxhp;
	float nowhp = 100.0f;
	float hpspeed = 0.1f;
	XMFLOAT2 spriteSize{ 400.0f, 50.0f };
	bool deadflag;
	ObjFile* obj = new ObjFile();
	Sprite* sprite = nullptr;
	Sprite* sprite2 = nullptr;
	Text* text = nullptr;
	ParticleManager* particle = nullptr;
};

