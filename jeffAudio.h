#pragma once

#include <xaudio2.h>
#include <unordered_map>
#include <string>
#include <stdexcept>

class jeffAudio
{
public:

	void loadSound(LPCWSTR filename);

	void playSound(LPCWSTR filename);

	static void makeInstance() { instance = new jeffAudio(); }
	static jeffAudio* getInstance() { return instance; }
	static void destroyInstance() { delete instance; }

private:
	jeffAudio()
	{
		HRESULT hr = XAudio2Create(&jAudio, 0, XAUDIO2_DEFAULT_PROCESSOR);
		hr = jAudio->CreateMasteringVoice(&jMasterVoice);
	}

	~jeffAudio()
	{
		jAudio->Release();
	}

	static inline jeffAudio* instance = nullptr;

	IXAudio2* jAudio = nullptr;
	IXAudio2MasteringVoice* jMasterVoice = nullptr;
	IXAudio2SourceVoice* jSourceVoice = nullptr;

	std::unordered_map<std::wstring, XAUDIO2_BUFFER> bufferMap;

	HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);

	HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD bufSize, DWORD bufOffset);

};