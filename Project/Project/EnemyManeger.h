#include"Enemy.h"
#include"EnemyAI.h"
#include"Poly.h"

class EnemyManeger
{
public:
	void Initialize();//������
	void Add(Enemy* enemy);//�}�l�[�W���[�ɓo�^
	void Create();//�}�l�[�W���[�ō쐬
	void Destory();//�폜
	void Draw();//�`��
	void Update();//�X�V����

private:
	Enemy* enemybox[100];
	int boxcount = 0;
};
