#pragma once
#include <DirectXMath.h>
#include <d3d12.h>

///<summary>
///���C�g
///</summary>
class Light
{
private://�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public://�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMVECTOR lightv; // ���C�g�ւ̕�����\���x�N�g��
		XMFLOAT3 lightcolor; // ���C�g�̐F
	};

private://�ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* device;

public://�ÓI�����o�֐�
	///<summary>
	///�ÓI������
	///</summary>
	///<param name="device">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* device);

private://�����o�ϐ�
	//�ϐ��o�b�t�@
	ComPtr<ID3D12Resource>constBuff;
	//���C�g��������(�P�ʃx�N�g��)
	XMVECTOR lightdir = { 0,-1, 0,0 };
	//���C�g�F
	XMFLOAT3 lightcolor = { 1,1,1 };
	//�_�[�e�B�t���O
	bool dirty = false;

public://�����o�֐�
	///<summary>
	///������
	///</summary>
	void Initialize();

	///<summary>
	///�X�V
	///</summary>
	void Update();

	///<summary>
	///�萔�o�b�t�@����
	///</summary>
	void CreateConstBuffer();

	///<summary>
	///�萔�o�b�t�@�]��
	///</summary>
	void TransferConstBuffer();

	///<summary>
	///���C�g�������Z�b�g
	///</summary>
	///<param name="lightdir">���C�g����</param>
	void SetLightDir(const XMVECTOR& lightdir);

	///<summary>
	///���C�g�̐F���Z�b�g
	///</summary>
	///<param name="lightcolor">���C�g�̐F</param>
	void SetLightColor(const XMFLOAT3& lightcolor);

	///<summary>
	///�`��
	///</summaryy>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

	///<summary>
	///�C���X�^���X����
	///</summary>
	///<returns>�C���X�^���X</returns>
	static Light* Create();
};
