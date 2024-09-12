#pragma once

#include <xaudio2.h>
#include <unordered_map>
#include <string>
#include <stdexcept>

namespace jeffNamespace
{
	class jeffAudio
	{
	public:
		void loadSound(LPCWSTR filename);
		void playSound(LPCWSTR filename, bool startImmediately = true);

		static void makeInstance() { instance = new jeffAudio(); }
		static jeffAudio* getInstance() { return instance; }
		static void destroyInstance() { delete instance; }

	private:
		struct jeffSound
		{
			IXAudio2SourceVoice* sourceVoice;
			XAUDIO2_BUFFER buf;
		};

		jeffAudio();
		~jeffAudio()
		{
			jAudio->Release();
		}

		static inline jeffAudio* instance = nullptr;

		IXAudio2* jAudio = nullptr;
		IXAudio2MasteringVoice* jMasterVoice = nullptr;

		std::unordered_map<std::wstring, jeffSound> soundMap;

		HRESULT FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition);
		HRESULT ReadChunkData(HANDLE hFile, void* buffer, DWORD bufSize, DWORD bufOffset);

	};
}