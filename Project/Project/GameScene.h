#pragma once
class GameScene
{
public:
	virtual void Update();
	virtual void Draw();
	virtual void Initialize();
	bool endFlag = false;
};

