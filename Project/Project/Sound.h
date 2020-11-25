#pragma once

#include <Windows.h>
#include <xaudio2.h>
#include <wrl.h>
#include <dshow.h>
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
	IGraphBuilder	*pGraphBuilder;
	IMediaControl	*pMediaControl;		//ファイルの読み込み、再生停止などを行う。
	IMediaPosition	*pMediaPosition;	//再生位置を指定するのに使う。

	bool isPlay;
	bool isRoopMode;

public:
	// チャンクヘッダ
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

	// 初期化
	void Initialize();

	void Update();

	void PlayWave(const char* filename);

	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	XAudio2VoiceCallBack voicecallback;

public:
	Sound()
	{
		pGraphBuilder = NULL;
		pMediaControl = NULL;
		pMediaPosition = NULL;
	
		isPlay = false;
		isRoopMode = false;
	}
	~Sound() { Release(); }

	// ファイルの読み込み
	void LoadFile(const wchar_t* filename);

	// 再生
	void Play();

	// ループ再生
	void PlayRoop();
	void ChkRoop();

	// 先頭に戻して再生し直し
	void PlayStart();

	// 一時停止
	void Pause();

	// 停止
	void Stop();

	// リソース解放
	void Release();
};