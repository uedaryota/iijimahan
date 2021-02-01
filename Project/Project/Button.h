#pragma once
#include"ObjFile.h"

class Button
{
public:
	Button();
	~Button();
	void Initialize();
	void Update();
	void Draw();

private:

	int ButtonCount = 7;

	ObjFile* buttons[7];
};

