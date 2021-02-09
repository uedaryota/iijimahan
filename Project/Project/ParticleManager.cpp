#include "ParticleManager.h"
ParticleManager::ParticleManager()
{
}

ParticleManager::~ParticleManager()
{
}

void ParticleManager::Update()
{
	for (auto a : particleList)
	{
	//	if (a->life > 0)
		{
			a->Update();
		}
//		else
		{
	//		Reset();
		}
	}
}

void ParticleManager::Draw()
{
	for (auto a : particleList)
	{
		//if (a->life > 0)
		{
			a->Draw();
		}
	}
}

void ParticleManager::Initialize()
{
	if (particleList.size() != particleCountMax)
	{
		for (int a = 0; a < particleCountMax; a++)
		{
			Particle* p = new Particle();
			particleList.push_back(p);
		}
	}

	for (auto a : particleList)
	{
		a->Initialize();
		a->SetPos(position);
		scale = 150;
		a->SetScale({ scale,scale,scale });
		a->life = 100;
		a->color = { 1,0,0,1 };
		a->speed = (rand() % 10);
		a->speed = a->speed / 10;
		float velX = rand() % 50;
		float velY = rand() % 50;
		if (rand() % 2 == 0)
		{
			velX *= -1;
		}
		if (rand() % 2 == 0)
		{
			velY *= -1;
		}
		float len = sqrtf(velX * velX + velY * velY);
		velX = velX / len;
		velY = velY / len;

		a->velocity = { velX,velY,0 };
	

	}
	Reset();
	//particleList
}

void ParticleManager::Reset()
{
	for (auto a : particleList)
	{
		a->life = 0;
		a->SetPos(position);
		a->velocity = { 0,0,0 };
		a->color = { 1,1,1,0 };
	}
}

void ParticleManager::Start()
{
	for (auto a : particleList)
	{
		a->SetPos(position);
		a->life = 150 - rand() % 50;

		float r, g, b;
		//Šî€
		a->maxColor = 1 - static_cast<float>(rand() % 2) / 10;
		a->minColor = static_cast<float>(rand() % 4) / 10;
		r = a->maxColor;
		g = r * 0.8f;
		b = r * 0.6f;
		a->color = { r,g,b,1 };

	//	a->scale = { static_cast<float> (rand() % 3 * 30), static_cast<float>(rand() % 3 * 30), 30 };

		//a->speed = scale / 115;
		a->speed = (rand() % 10) * 5;
		a->speed = a->speed / 10;
	
		
		float value = scale / 350;
		a->speed = a->speed*value;
		a->addspeed = -(a->speed / 10);
		float velX = rand() % 50;
		float velY = rand() % 50;
		if (rand() % 2 == 0)
		{
			velX *= -1;
		}
		if (rand() % 2 == 0)
		{
			velY *= -1;
		}
		float len = sqrtf(velX * velX + velY * velY);
		velX = velX / len;
		velY = velY / len;

		a->velocity = { velX,velY,0 };
	}
}

void ParticleManager::SetPos(XMFLOAT3 pos)
{
	position = pos;
}
