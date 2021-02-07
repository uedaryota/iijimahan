#pragma once

#include "Sprite.h"
#include <Windows.h>
#include <string>

using namespace DirectX;

/// <summary>
/// デバッグ用文字表示
/// </summary>
class RedText
{
public:
	// デバッグテキスト用のテクスチャ番号を指定
	static const int maxCharCount = 256;	// 最大文字数
	static const int fontWidth = 9;			// フォント画像内1文字分の横幅
	static const int fontHeight = 18;		// フォント画像内1文字分の縦幅
	static const int fontLineCount = 14;	// フォント画像内1行分の文字数
	static const int bufferSize = 256;	// 書式付き文字列展開用バッファサイズ

public:// 静的メンバ関数
	static RedText* GetInstance();

public:// メンバ関数

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
	// スプライトデータの配列
	Sprite* spriteDatas[maxCharCount] = {};
	// スプライトデータ配列の添え字番号
	int spriteIndex = 0;

	float posX = 0.0f;
	float posY = 0.0f;
	float size = 1.0f;

	XMFLOAT4 color = XMFLOAT4(1, 1, 1, 1);

	char buffer[bufferSize];
};
