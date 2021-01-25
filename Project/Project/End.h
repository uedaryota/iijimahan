#pragma once
#include"GameScene.h"
#include"Camera.h"
#include"Sprite.h"
class End:public GameScene
{
public:
	End();
	void Draw() override;
	void Update() override;
	void Initialize() override;

	Sprite* sprite1 = nullptr;
};

