#pragma once
#include"GameScene.h"
#include"Sprite.h"
#include"ObjFile.h"
#include"Camera.h"
#include"Sound.h"
#include "Text.h"
#include"Input.h"
class Title:public GameScene
{
public:
	Title();
	void Draw() override;
	void Update() override;
	void Initialize() override;

	ObjFile* obj = new ObjFile();
	Sprite* sprite;
	Sprite* sprite2;
	Sound* sound = new Sound();
	Input* input2 = new Input();
private:
	Text* text;
};

