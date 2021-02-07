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

	int ButtonCount = 6;

	ObjFile* buttons[7];
	ObjFile* black_buttons[7];

public:
	bool costflag;
};

