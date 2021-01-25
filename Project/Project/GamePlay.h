#pragma once
#include"GameScene.h"

enum WAVE
{
	wave1, wave2, wave3, clear
};
class GamePlay:public GameScene
{
public:
	GamePlay();
	void Draw() override;
	void Update() override;
	void Initialize() override;
	void CollisionUpdate();
	WAVE NowWAVE = wave1;
	void CostUpdate();
};

