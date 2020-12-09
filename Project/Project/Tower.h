#pragma once
#include"Camera.h"
#include"ObjFile.h"
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include"Texture.h"
class Tower
{
public:
	Tower();
	~Tower();
	void Initialize(ID3D12Device* dev);
	void Update();
	void Draw(ID3D12GraphicsCommandList * cmdList);
	XMFLOAT3 GetPosition();//11/4エネミー用に位置を送ります。byヤマナカ
	void SetPoisition(XMFLOAT3 position);//11/11アンカーように位置変更をできるようにします。
	float GetHp();
	void SetHp(float x);
	void Damage(float damage);
	float hp =100.0f;
private:
	XMFLOAT3 texSize{ 20,50,0 };
	XMFLOAT3 texPos{ 0,0,0 };
	bool deadflag;
	ObjFile* obj = new ObjFile();
	Texture* tex = new Texture();
};

