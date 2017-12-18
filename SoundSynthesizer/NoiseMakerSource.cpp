#include "NoiseMakerHeader.h"

#include "mmeapi.h"
#include <cmath>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <Windows.h>

using namespace std;


template<class T>NoiseMaker::NoiseMaker(wstring sOutputDevice, unsigned int nSampleRate = 44100, unsigned int nChannels = 1, unsigned int nBlocks = 8, unsigned int nBlockSamples = 512) {
	Create(sOutputDevicem, nSampleRate, nChannels, nBlocks, nBlockSamples);
	// Creates itself
}

template<class T>NoiseMaker::~NoiseMaker() {
	Destroy(); // trash collection
}

template<class T> bool NoiseMaker::Create(wstring sOutputDevice, unsigned int nSampleRate = 44100, unsigned int nChannels = 1, unsigned int nBlocks = 8, unsigned int nBlockSamples = 512) {
	m_bReady = false;
	m_nSampleRate = nSampleRate;
	m_nChannels = nChannels;
	m_nBlockCount = nBlocks;
	m_nBlockFree = m_nBlockCount;
	m_nBlockCurrent = 0;
	m_pBlockMemory = nullptr;
	m_pWaveHeaders = nullptr;
	m_userFunction = nullptr;
	vector<wstring> devices = Enumerate();
	auto d = std::find(devices.begin(), devices.end(), sOutputDevice);
	if (d != devices.end()) {
		int nDeviceID = distance(devices.begin(), d);
		WAVEFORMATEX waveFormat;
		waveFormat.wFormatTag = WAVE_FORMAT_PCM;
		waveFormat.nSamplesPerSec = m_nSampleRate;
		waveFormat.wBitsPerSample = sizeof(T) * 8;
		waveFormat.nChannels = m_nChannels;
		waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
		waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
		waveFormat.cbSize = 0;
		if (waveOutOpen(&m_hwDevice, nDeviceID, &waveFormat, (DWORD_PTR)waveOutProcWrap, (DWORD_PTR)this, CALLBACK_FUNCTION) != S_OK) {
			return Destroy();
		}
	}
}
