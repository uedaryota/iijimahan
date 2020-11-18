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
	//�{�C�X�����p�X�̊J�n��
	STDMETHOD_(void, OnVoiceProcessingPassStart)(THIS_ UINT32 BytesRequired) {};
	//�{�C�X�����p�X�̏I����
	STDMETHOD_(void, OnVoiceProcessingPassEnd)(THIS) {};
	//�o�b�t�@�X�g���[���̍Đ����I��������
	STDMETHOD_(void, OnStreamEnd)(THIS) {};
	//�o�b�t�@�̎g�p�J�n��
	STDMETHOD_(void, OnBufferStart)(THIS_ void* pBufferContext) {};
	//�o�b�t�@�̖����ɒB������
	STDMETHOD_(void, OnBufferEnd)(THIS_ void* pBufferContext)
	{
		//�o�b�t�@���������
		delete[] pBufferContext;
	};
	//�Đ������[�v�ʒu�ɒB������
	STDMETHOD_(void, OnLoopEnd)(THIS_ void* pBufferContext) {};
	//�{�C�X�̎��s�G���[��
	STDMETHOD_(void, OnVoiceError)(THIS_ void* pBufferContext, HRESULT Error) {};
};

class Sound
{
	IGraphBuilder	*pGraphBuilder;
	IMediaControl	*pMediaControl;		//�t�@�C���̓ǂݍ��݁A�Đ���~�Ȃǂ��s���B
	IMediaPosition	*pMediaPosition;	//�Đ��ʒu���w�肷��̂Ɏg���B

	bool isPlay;
	bool isRoopMode;

public:
	// �`�����N�w�b�_
	struct Chunk
	{
		char id[4];  //�`�����N����ID
		int size;//�`�����N�T�C�Y
	};

	// RIFF�w�b�_�`�����N
	struct RiffHeader
	{
		Chunk chunk;  //"RIFF"
		char type[4]; //"WAVE"
	};

	// FMT�`�����N
	struct FormatChunk
	{
		Chunk chunk;    //"fmt"
		WAVEFORMAT fmt; //�g�`�t�H�[�}�b�g
	};

	// ������
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

	// �t�@�C���̓ǂݍ���
	void LoadFile(BSTR file);

	// �Đ�
	void Play();

	// ���[�v�Đ�
	void PlayRoop();
	void ChkRoop();

	// �擪�ɖ߂��čĐ�������
	void PlayStart();

	// �ꎞ��~
	void Pause();

	// ��~
	void Stop();

	// ���\�[�X���
	void Release();
};