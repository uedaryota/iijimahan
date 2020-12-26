#pragma once
#include"Tower.h"
#include"Battery.h"
#include"Enemy.h"
#include"Input.h"
#include"CircleCollision.h"
#include"Camera.h"
class Collision
{
public:
	Collision();
	~Collision();

	void Update();
	void Draw();
	void Initialize();
	bool MouseToCircle(Input input, CircleCollision circle);
};

