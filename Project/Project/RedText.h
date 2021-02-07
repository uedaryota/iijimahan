#pragma once

#include "Sprite.h"
#include <Windows.h>
#include <string>

using namespace DirectX;

/// <summary>
/// �f�o�b�O�p�����\��
/// </summary>
class RedText
{
public:
	// �f�o�b�O�e�L�X�g�p�̃e�N�X�`���ԍ����w��
	static const int maxCharCount = 256;	// �ő啶����
	static const int fontWidth = 9;			// �t�H���g�摜��1�������̉���
	static const int fontHeight = 18;		// �t�H���g�摜��1�������̏c��
	static const int fontLineCount = 14;	// �t�H���g�摜��1�s���̕�����
	static const int bufferSize = 256;	// �����t��������W�J�p�o�b�t�@�T�C�Y

public:// �ÓI�����o�֐�
	static RedText* GetInstance();

public:// �����o�֐�

	void Initialize(UINT texnumber);

	inline void SetPos(float x, float y) {
		posX = x;
		posY = y;
	}

	inline void SetSize(float size) { this->size = size; }

	void Print(const std::string & text, float x, float y, float size);
	void NPrint(int len, const char* text);
	void Printf(const char* fmt, ...);

	void DrawAll(ID3D12GraphicsCommandList * cmdList);

	inline void SetColor(XMFLOAT4 color) { this->color = color; }

private:
	RedText();
	RedText(const RedText&) = delete;
	~RedText();
	RedText& operator=(const RedText&) = delete;

private:
	// �X�v���C�g�f�[�^�̔z��
	Sprite* spriteDatas[maxCharCount] = {};
	// �X�v���C�g�f�[�^�z��̓Y�����ԍ�
	int spriteIndex = 0;

	float posX = 0.0f;
	float posY = 0.0f;
	float size = 1.0f;

	XMFLOAT4 color = XMFLOAT4(1, 1, 1, 1);

	char buffer[bufferSize];
};
