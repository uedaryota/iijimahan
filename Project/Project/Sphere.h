#pragma once
#include "Camera.h"
#include "ObjFile.h"
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

class Sphere
{
public:
	Sphere();
	~Sphere();
	void Initialize(ID3D12Device* dev);
	void Update();
	void Draw(ID3D12GraphicsCommandList * cmdList);
	void SetPos(XMFLOAT3 pos);
	void SetRot(XMFLOAT3 rot);
	void SetScale(XMFLOAT3 scale);
	XMFLOAT3 position{};
	XMFLOAT3 rotation{};
	XMFLOAT3 scale{};

private:
	ObjFile* obj = new ObjFile();
};

