#include"Enemy.h"
#include"EnemyAI.h"
#include"Poly.h"
#include"Spawn.h"

class EnemyManeger
{
public:
	void Initialize();//������
	void Add(Enemy* enemy);//�}�l�[�W���[�ɓo�^
	void Add2();//�}�l�[�W���[�ɓo�^
	void Create();//�}�l�[�W���[�ō쐬
	void Destory();//�폜
	void Draw(ID3D12GraphicsCommandList * cmdList);//�`��
	void Update();//�X�V����

    ///<summary>
	///��1��2�|�C���g�܂Ƃ߂Ă̈ړ��ڕW�̑S�̐ݒ�
	///<param name="anc1">��1�ړ��n�_</param>
	///<param name="anc2">��2�ړ��n�_</param>
	///</summary>
	void AncerSet(XMFLOAT3 anc1,XMFLOAT3 anc2);

	///<summary>
	///��1�|�C���g�̑S�̐ݒ�
	///<param name="anc1">��1�ړ��n�_</param>
	///</summary>
	void FirstAncerSet(XMFLOAT3 anc1);

	///<summary>
	///��2�|�C���g�̑S�̐ݒ�
	///<param name="anc2">��2�ړ��n�_</param>
	///</summary>
	void SecondAncerSet(XMFLOAT3 anc2);

	///<summary>
	///��1��2�|�C���g�܂Ƃ߂Ă̈ړ��ڕW�̍Đݒ�
	///<param name="anc1">��1�ړ��n�_</param>
	///<param name="anc2">��2�ړ��n�_</param>
	///</summary>
	void ReAncerSet(XMFLOAT3 anc1, XMFLOAT3 anc2);

	bool Serch();
	State Bank;

private:
	Enemy* enemybox[999];
	int boxcount = 0;
	Spawn* spawn=new Spawn();
	int amount;
	float timer = 0;
	int Cnt = 0;
};
