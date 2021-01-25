#pragma once
#include"GameScene.h"
class GamePlay:public GameScene
{
public:
	GamePlay();
	void Draw() override;
	void Update() override;
	void Initialize() override;
	void CollisionUpdate();
	void CostUpdate();
	void EndFlagCheck();
	bool clearFlag = false;
	bool overFlag = false;
};

