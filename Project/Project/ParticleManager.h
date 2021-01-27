#pragma once
#include"Particle.h"
#include<vector>
class ParticleManager
{
public:
	ParticleManager();
	~ParticleManager();
	void Update();
	void Draw();
	void Initialize();
	void Reset();
	void Start();
	void SetPos(XMFLOAT3 pos);
	XMFLOAT3 position = { 0,0,0 };
	int particleCountMax = 100;
	int particleCountMin = 10;
	float scale;
	std::vector<Particle*>particleList;
};

