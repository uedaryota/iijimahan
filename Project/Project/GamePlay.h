#pragma once
#include"GameScene.h"
#include"Sound.h"

enum WAVE
{
	wave1, wave2, wave3, wave0
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
	void EndFlagCheck();
	bool clearFlag = false;
	bool overFlag = false;
	Sound* sound = new Sound();
};

