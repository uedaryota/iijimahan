#pragma once

#include <Windows.h>
#include <xaudio2.h>
#include <wrl.h>
#include "Input.h"

#pragma comment(lib, "xaudio2.lib")

using namespace Microsoft::WRL;

class XAudio2VoiceCallBack : public IXAudio2VoiceCallback
{
public:
	//ボイス処理パスの開始時
	STDMETHOD_(void, OnVoiceProcessingPassStart)(THIS_ UINT32 BytesRequired) {};
	//ボイス処理パスの終了時
	STDMETHOD_(void, OnVoiceProcessingPassEnd)(THIS) {};
	//バッファストリームの再生が終了した時
	STDMETHOD_(void, OnStreamEnd)(THIS) {};
	//バッファの使用開始時
	STDMETHOD_(void, OnBufferStart)(THIS_ void* pBufferContext) {};
	//バッファの末尾に達した時
	STDMETHOD_(void, OnBufferEnd)(THIS_ void* pBufferContext)
	{
		//バッファを解放する
		delete[] pBufferContext;
	};
	//再生がループ位置に達した時
	STDMETHOD_(void, OnLoopEnd)(THIS_ void* pBufferContext) {};
	//ボイスの実行エラー時
	STDMETHOD_(void, OnVoiceError)(THIS_ void* pBufferContext, HRESULT Error) {};
};

class Sound
{
public:
	//チャンクヘッダ
	struct Chunk
	{
		char id[4];  //チャンク毎のID
		int size;//チャンクサイズ
	};

	// RIFFヘッダチャンク
	struct RiffHeader
	{
		Chunk chunk;  //"RIFF"
		char type[4]; //"WAVE"
	};

	// FMTチャンク
	struct FormatChunk
	{
		Chunk chunk;    //"fmt"
		WAVEFORMAT fmt; //波形フォーマット
	};

	//初期化
	void Initialize();

	void Update();

	void PlayWave(const char* filename);

	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	XAudio2VoiceCallBack voicecallback;
};