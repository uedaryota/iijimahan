#pragma once
#include"GameScene.h"
class Title:public GameScene
{
public:
	Title();
	void Draw() override;
	void Update() override;
	void Initialize() override;

};

