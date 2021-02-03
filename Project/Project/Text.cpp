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
	// 全てのスプライトデータについて
	for (int i = 0; i < _countof(spriteDatas); i++)
	{
		// スプライトを生成する
		spriteDatas[i] = Sprite::Create(texnumber, { 0,0 });
	}
}

// 1文字列追加
void Text::Print(const std::string & text, float x, float y, float scale = 1.0f)
{
	SetPos(x, y);
	SetSize(scale);

	NPrint(text.size(), text.c_str());
}

void Text::NPrint(int len, const char* text)
{
	// 全ての文字について
	for (int i = 0; i < len; i++)
	{
		// 最大文字数超過
		if (spriteIndex >= maxCharCount) {
			break;
		}

		// 1文字取り出す(※ASCIIコードでしか成り立たない)
		const unsigned char& character = text[i];

		int fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;

		// 座標計算
		spriteDatas[spriteIndex]->SetPosition({ this->posX + fontWidth * this->size * i, this->posY });
		spriteDatas[spriteIndex]->SetTextureRect({ (float)fontIndexX * fontWidth, (float)fontIndexY * fontHeight }, { (float)fontWidth, (float)fontHeight });
		spriteDatas[spriteIndex]->SetSize({ fontWidth * this->size, fontHeight * this->size });
		spriteDatas[spriteIndex]->SetColor({ this->color });

		// 文字を１つ進める
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

// まとめて描画
void Text::DrawAll(ID3D12GraphicsCommandList* cmdList)
{
	// 全ての文字のスプライトについて
	for (int i = 0; i < spriteIndex; i++)
	{
		// スプライト描画
		spriteDatas[i]->Draw();
	}

	spriteIndex = 0;
}