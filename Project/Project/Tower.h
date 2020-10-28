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
private:
	
	int hp;
	bool deadflag;
	ObjFile* obj = new ObjFile();
};

