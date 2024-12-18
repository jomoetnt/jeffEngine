#include "jeffAudio.h"

using namespace jeffNamespace;

jeffAudio::jeffAudio()
{
	HRESULT hr = XAudio2Create(&jAudio, 0, XAUDIO2_DEFAULT_PROCESSOR);
	hr = jAudio->CreateMasteringVoice(&jMasterVoice);
	jMasterVoice->SetVolume(0.4f);
}

void jeffAudio::loadSound(LPCWSTR filename)
{
	WAVEFORMATEXTENSIBLE wfx{};
	HANDLE hfile = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	DWORD fPtr = SetFilePointer(hfile, 0, NULL, FILE_BEGIN);

	DWORD chunkSize; DWORD chunkPos;
	HRESULT hr = FindChunk(hfile, 'FFIR', chunkSize, chunkPos);
	DWORD filetype;
	hr = ReadChunkData(hfile, &filetype, sizeof(DWORD), chunkPos);
	if (filetype != 'EVAW') hr = S_FALSE;

	hr = FindChunk(hfile, ' tmf', chunkSize, chunkPos);
	hr = ReadChunkData(hfile, &wfx, chunkSize, chunkPos);

	hr = FindChunk(hfile, 'atad', chunkSize, chunkPos);
	BYTE* dataBuf = new BYTE[chunkSize];
	hr = ReadChunkData(hfile, dataBuf, chunkSize, chunkPos);

	IXAudio2SourceVoice* source = nullptr;
	hr = jAudio->CreateSourceVoice(&source, (WAVEFORMATEX*)&wfx);
	if (FAILED(hr)) throw std::runtime_error("error creating source voice");

	jeffSound newSound{};
	newSound.buf.AudioBytes = chunkSize; newSound.buf.pAudioData = dataBuf; newSound.buf.Flags = XAUDIO2_END_OF_STREAM;
	newSound.sourceVoice = source;
	soundMap[filename] = newSound;
}

void jeffAudio::playSound(LPCWSTR filename, bool startImmediately)
{
	if (startImmediately)
	{
		soundMap[filename].sourceVoice->Stop();
		soundMap[filename].sourceVoice->FlushSourceBuffers();
	}
	HRESULT hr = soundMap[filename].sourceVoice->SubmitSourceBuffer(&soundMap[filename].buf);
	if (FAILED(hr)) throw std::runtime_error("error submitting source buffer");
	hr = soundMap[filename].sourceVoice->Start();
	if (FAILED(hr)) throw std::runtime_error("error playing sound");
}

HRESULT jeffAudio::FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
	{
		if (FAILED(hr)) throw std::runtime_error("error finding chunk");
		return HRESULT_FROM_WIN32(GetLastError());
	}

	DWORD dwChunkType = 0;
	DWORD dwChunkDataSize = 0;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType = 0;
	DWORD bytesRead = 0;
	DWORD dwOffset = 0;

	while (hr == S_OK)
	{
		DWORD dwRead;
		if (0 == ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		if (0 == ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL))
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (0 == ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL))
				hr = HRESULT_FROM_WIN32(GetLastError());
			break;

		default:
			if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT))
			{
				if (FAILED(hr)) throw std::runtime_error("error finding chunk");
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize)
		{
			if (FAILED(hr)) throw std::runtime_error("error finding chunk");
			return S_FALSE;
		}
	}

	return S_OK;
}

HRESULT jeffAudio::ReadChunkData(HANDLE hFile, void* buffer, DWORD bufSize, DWORD bufOffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufOffset, NULL, FILE_BEGIN))
	{
		if (FAILED(hr)) throw std::runtime_error("error finding chunk");
		return HRESULT_FROM_WIN32(GetLastError());
	}
	DWORD read;
	if (0 == ReadFile(hFile, buffer, bufSize, &read, NULL))
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		if (FAILED(hr)) throw std::runtime_error("error finding chunk");
	}
	return hr;
}