#include "Sound.h"
#include <fstream>
#include <cassert>
#include <tchar.h>
#include <comutil.h>

#pragma comment(lib, "strmiids.lib")

void Sound::Initialize()
{
	HRESULT result;

	//XAudio2�G���W���̃C���X�^���X�𐶐�
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if FAILED(result)
	{
		assert(0);
	}

	//�}�X�^�[�{�C�X�𐶐�
	result = xAudio2->CreateMasteringVoice(&masterVoice); 
	if FAILED(result)
	{
		assert(0);
	}
}

void Sound::Update()
{
	Input* input = new Input();
	input->Initialize();
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

//Wave�Đ�
void Sound::PlayWave(const char* filename)
{
	HRESULT result;

#pragma region �t�@�C���I�[�v��

	//�t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;
	//.wave�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(filename, std::ios_base::binary);
	//�t�@�C���I�[�v�����s�����o����
	if (file.fail())
	{
		assert(0);
	}

#pragma endregion

#pragma region .wav�t�@�C���̓ǂݍ��ݏ���

	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	//�t�@�C����RIFF���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}

	//Format�`�����N�̓ǂݍ���
	FormatChunk format;
	file.read((char*)&format, sizeof(format));

	//Data�`�����N�̓ǂݍ���
	Chunk data;
	file.read((char*)&data, sizeof(data));
	
	//Data�`�����N�̃f�[�^��(�g�`�f�[�^)�̓ǂݍ���
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//Wave�t�@�C�������
	file.close();

#pragma endregion

#pragma region �T�E���h�Đ�

	WAVEFORMATEX wfex{};

	//�g�`�t�H�[�}�b�g�̐ݒ�
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	//�g�`�t�H�[�}�b�g����SourceVoice�̐���
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &wfex, 0, 2.0f, &voicecallback);
	if FAILED(result)
	{
		delete[] pBuffer;
		return;
	}

	//�Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = (BYTE*)pBuffer;
	buf.pContext = pBuffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = data.size;

	//�g�`�f�[�^�̍Đ�
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();

#pragma endregion
}

//�t�@�C���̓ǂݍ���
void Sound::LoadFile(const wchar_t* filename)
{
	CoInitialize(NULL);
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (LPVOID *)&pGraphBuilder);

	pGraphBuilder->QueryInterface(IID_IMediaControl, (LPVOID *)&pMediaControl);
	pGraphBuilder->QueryInterface(IID_IMediaPosition, (LPVOID *)&pMediaPosition);
	
	BSTR soundfile = ::SysAllocString(filename);
	pMediaControl->RenderFile(soundfile);
}

// �ʏ�Đ�
void Sound::Play()
{
	pMediaControl->Run();
	isPlay = true;
	isRoopMode = false;
}

// ���[�v�Đ�
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

// �擪�ɖ߂��čĐ�������
void Sound::PlayStart()
{
	pMediaPosition->put_CurrentPosition(0);
	pMediaControl->Run();

	isPlay = true;
}

// �ꎞ��~
void Sound::Pause()
{
	pMediaControl->Pause();

	isPlay = false;
}

// ��~
void Sound::Stop()
{
	pMediaControl->Stop();
	pMediaPosition->put_CurrentPosition(0);

	isPlay = false;
}

// ���\�[�X���
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