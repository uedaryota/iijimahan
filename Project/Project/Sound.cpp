#include "Sound.h"
#include <fstream>
#include <cassert>
#include <tchar.h>
#include <comutil.h>

#pragma comment(lib, "strmiids.lib")

void Sound::Initialize()
{
	HRESULT result;

	//XAudio2エンジンのインスタンスを生成
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if FAILED(result)
	{
		assert(0);
	}

	//マスターボイスを生成
	result = xAudio2->CreateMasteringVoice(&masterVoice); 
	if FAILED(result)
	{
		assert(0);
	}
	input = new Input();
	input->Initialize();
	
}

void Sound::Update()
{
	input->Update();
	if (input->PushKey(DIK_SPACE))
	{
		PlayWave("Resources/Alarm01.wav");
	}
	//if (input->PushKey(DIK_P))
	//{
	//	PlayRoop();
	//}
	
}

//Wave再生
void Sound::PlayWave(const char* filename)
{
	HRESULT result;

#pragma region ファイルオープン

	//ファイル入力ストリームのインスタンス
	std::ifstream file;
	//.waveファイルをバイナリモードで開く
	file.open(filename, std::ios_base::binary);
	//ファイルオープン失敗を検出する
	if (file.fail())
	{
		assert(0);
	}

#pragma endregion

#pragma region .wavファイルの読み込み処理

	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	//ファイルがRIFFかチェック
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}

	//Formatチャンクの読み込み
	FormatChunk format;
	file.read((char*)&format, sizeof(format));

	//Dataチャンクの読み込み
	Chunk data;
	file.read((char*)&data, sizeof(data));
	
	//Dataチャンクのデータ部(波形データ)の読み込み
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//Waveファイルを閉じる
	file.close();

#pragma endregion

#pragma region サウンド再生

	WAVEFORMATEX wfex{};

	//波形フォーマットの設定
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	//波形フォーマットを元SourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &wfex, 0, 2.0f, &voicecallback);
	if FAILED(result)
	{
		delete[] pBuffer;
		return;
	}

	//再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = (BYTE*)pBuffer;
	buf.pContext = pBuffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = data.size;

	//波形データの再生
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();

#pragma endregion
}

//ファイルの読み込み
void Sound::LoadFile(const wchar_t* filename)
{
	CoInitialize(NULL);
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (LPVOID *)&pGraphBuilder);

	pGraphBuilder->QueryInterface(IID_IMediaControl, (LPVOID *)&pMediaControl);
	pGraphBuilder->QueryInterface(IID_IMediaPosition, (LPVOID *)&pMediaPosition);
	
	BSTR soundfile = ::SysAllocString(filename);
	pMediaControl->RenderFile(soundfile);
}

// 通常再生
void Sound::Play()
{
	pMediaControl->Run();
	isPlay = true;
	isRoopMode = false;
}

// ループ再生
void Sound::PlayRoop()
{
	pMediaControl->Run();
	isPlay = true;
	isRoopMode = true;
}

void Sound::ChkRoop()
{
	if (!isPlay || !isRoopMode) { return; }

	REFTIME pos, end;
	pMediaPosition->get_CurrentPosition(&pos);
	pMediaPosition->get_StopTime(&end);
	if (pos >= end)
	{
		Stop();
		PlayRoop();
	}
}

// 先頭に戻して再生し直し
void Sound::PlayStart()
{
	pMediaPosition->put_CurrentPosition(0);
	pMediaControl->Run();

	isPlay = true;
}

// 一時停止
void Sound::Pause()
{
	pMediaControl->Pause();

	isPlay = false;
}

// 停止
void Sound::Stop()
{
	pMediaControl->Stop();
	pMediaPosition->put_CurrentPosition(0);

	isPlay = false;
}

// リソース解放
void Sound::Release()
{
	if (pMediaPosition)
	{
		pMediaPosition->Release();
		pMediaPosition = NULL;
	}

	if (pMediaControl)
	{
		pMediaControl->Release();
		pMediaControl = NULL;
	}

	if (pGraphBuilder)
	{
		pGraphBuilder->Release();
		pGraphBuilder = NULL;

		CoUninitialize();
	}
}

bool Sound::Nullcheck()
{
	pGraphBuilder;
	pMediaControl;
	pMediaPosition;
	if (pGraphBuilder == NULL || pMediaControl == NULL || pMediaPosition == NULL) return true;
	return false;
}
