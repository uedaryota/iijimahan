#pragma once
#include"GameScene.h"
#include"Sprite.h"
#include"ObjFile.h"
#include"Camera.h"
#include"Sound.h"
#include "Text.h"

class Title:public GameScene
{
public:
	Title();
	void Draw() override;
	void Update() override;
	void Initialize() override;

	ObjFile* obj;
	Sprite* sprite;
	Sprite* sprite2;
	Sound* sound = new Sound();

private:
	Text* text;
};

