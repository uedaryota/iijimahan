#include "Block.h"
#include"DirectXDevice.h"
Block::Block()
{
}

Block::~Block()
{
	delete(pol);
}

void Block::Initialize()
{
	pol->Initialize();
}

void Block::Update()
{
	pol->Update();
}

void Block::Draw()
{
	pol->Draw();
}

void Block::SetPos(XMFLOAT3 position)
{
	pol->position = position;
}

void Block::SetScale(XMFLOAT3 scale)
{
	pol->scale = scale;
}
