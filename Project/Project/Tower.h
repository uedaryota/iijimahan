#pragma once
#include"Camera.h"
#include"ObjFile.h"
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
class Tower
{
public:
	Tower();
	~Tower();
	void Initialize(ID3D12Device* dev);
	void Update();
	void Draw(ID3D12GraphicsCommandList * cmdList);
	XMFLOAT3 GetPosition();//11/4�G�l�~�[�p�Ɉʒu�𑗂�܂��Bby���}�i�J

private:
	
	int hp;
	bool deadflag;
	ObjFile* obj = new ObjFile();
};

