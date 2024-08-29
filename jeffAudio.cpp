#include "jeffAudio.h"


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

	bufferMap[filename].AudioBytes = chunkSize;
	bufferMap[filename].pAudioData = dataBuf;
	bufferMap[filename].Flags = XAUDIO2_END_OF_STREAM;

	hr = jAudio->CreateSourceVoice(&jSourceVoice, (WAVEFORMATEX*)&wfx);
}

void jeffAudio::playSound(LPCWSTR filename)
{
	HRESULT hr = jSourceVoice->SubmitSourceBuffer(&bufferMap[filename]);
	hr = jSourceVoice->Start(0);
}

HRESULT jeffAudio::FindChunk(HANDLE hFile, DWORD fourcc, DWORD& dwChunkSize, DWORD& dwChunkDataPosition)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, 0, NULL, FILE_BEGIN))
		return HRESULT_FROM_WIN32(GetLastError());

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
				return HRESULT_FROM_WIN32(GetLastError());
		}

		dwOffset += sizeof(DWORD) * 2;

		if (dwChunkType == fourcc)
		{
			dwChunkSize = dwChunkDataSize;
			dwChunkDataPosition = dwOffset;
			return S_OK;
		}

		dwOffset += dwChunkDataSize;

		if (bytesRead >= dwRIFFDataSize) return S_FALSE;

	}

	return S_OK;
}

HRESULT jeffAudio::ReadChunkData(HANDLE hFile, void* buffer, DWORD bufSize, DWORD bufOffset)
{
	HRESULT hr = S_OK;
	if (INVALID_SET_FILE_POINTER == SetFilePointer(hFile, bufOffset, NULL, FILE_BEGIN)) return HRESULT_FROM_WIN32(GetLastError());
	DWORD read;
	if (0 == ReadFile(hFile, buffer, bufSize, &read, NULL)) hr = HRESULT_FROM_WIN32(GetLastError());
	return hr;
}