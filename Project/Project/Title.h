#pragma once
#include"GameScene.h"
#include"Sprite.h"
#include"ObjFile.h"
#include"Camera.h"

class Title:public GameScene
{
public:
	Title();
	void Draw() override;
	void Update() override;
	void Initialize() override;

	ObjFile* obj;

};

