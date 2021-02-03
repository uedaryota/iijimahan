#include "Text.h"

Text::Text()
{
}

Text::~Text()
{
	for (int i = 0; i < _countof(spriteDatas); i++) {
		delete spriteDatas[i];
	}
}

Text * Text::GetInstance()
{
	static Text instance;
	return &instance;
}

void Text::Initialize(UINT texnumber)
{
	// �S�ẴX�v���C�g�f�[�^�ɂ���
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		// �X�v���C�g�𐶐�����
		spriteDatas[i] = Sprite::Create(texnumber, { 0,0 });
	}
}

// 1������ǉ�
void Text::Print(const std::string & text, float x, float y, float scale = 1.0f)
{
	SetPos(x, y);
	SetSize(scale);

	NPrint(text.size(), text.c_str());
}

void Text::NPrint(int len, const char* text)
{
	// �S�Ă̕����ɂ���
	for (int i = 0; i < len; i++)
	{
		// �ő啶��������
		if (spriteIndex >= maxCharCount) {
			break;
		}

		// 1�������o��(��ASCII�R�[�h�ł������藧���Ȃ�)
		const unsigned char& character = text[i];

		int fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// ���W�v�Z
		spriteDatas[spriteIndex]->SetPosition({ this->posX + fontWidth * this->size * i, this->posY });
		spriteDatas[spriteIndex]->SetTextureRect({ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight }, { (float)fontWidth, (float)fontHeight });
		spriteDatas[spriteIndex]->SetSize({ fontWidth * this->size, fontHeight * this->size });
		spriteDatas[spriteIndex]->SetColor({ this->color });

		// �������P�i�߂�
		spriteIndex++;
	}
}

void Text::Printf(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int w = vsnprintf(buffer, bufferSize - 1, fmt, args);
	NPrint(w, buffer);
	va_end(args);
}

// �܂Ƃ߂ĕ`��
void Text::DrawAll(ID3D12GraphicsCommandList* cmdList)
{
	// �S�Ă̕����̃X�v���C�g�ɂ���
	for (int i = 0; i < spriteIndex; i++)
	{
		// �X�v���C�g�`��
		spriteDatas[i]->Draw();
	}

	spriteIndex = 0;
}