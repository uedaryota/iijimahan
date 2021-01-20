#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include "Texture.h"
#include "Sprite.h"

class Cost
{
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �R�X�g�̎擾
	/// </summary>
	int GetCost();

	/// <summary>
	/// �R�X�g�̑���
	/// </summary>
	/// <param name="cost">�R�X�g</param>
	void SetAddCost(int cost) { this->cost += cost; }

	/// <summary>
	/// �R�X�g�̌���
	/// </summary>
	/// <param name ="cost">�R�X�g</param>
	void SetSubCost(int cost) { this->cost -= cost; }

	/// <summary>
	/// �R�X�g�����Ԋu�̂̐ݒ�
	/// </summary>
	/// <param name="costUpTime">�R�X�g�̑����Ԋu</param>
	void SetCostUpTime(int costUpTime) { this->costUpTime = costUpTime; }

private:
	int maxcost = 60;//�ő�R�X�g
	Sprite* sprite = new Sprite();

public:
	int costUpTime = 30;//�R�X�g�����Ԋu
	int time = 0;//���ԑ���p
	int cost = 0;//�K�W�F�b�g�ݒu�p�R�X�g
};

