#pragma once
#include"GameScene.h"
#include"Sound.h"
#include "Text.h"

enum WAVE
{
	wave1, wave2, wave3, wave4,wave0
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
	bool Boss = false;
private:
	Text * text;
	Sprite* nextback;
	static const int TextNumber = 0;
};

