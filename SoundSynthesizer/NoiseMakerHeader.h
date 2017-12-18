#pragma once
#pragma comment(lib, "winmm.lib")

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <Windows.h>

using namespace std;

const double PI = 2.0 * acos(0.0);

template<class T>
abstract static class NoiseMaker {
	public:
		NoiseMaker(wstring sOutputDevice, unsigned int nSampleRate = 44100, unsigned int nChannels = 1, unsigned int nBlocks = 8, unsigned int nBlockSamples = 512);
		~NoiseMaker();
		virtual bool Create(wstring sOutputDevice, unsigned int nSampleRate = 44100, unsigned int nChannels = 1, unsigned int nBlocks = 8, unsigned int nBlockSamples = 512);
		bool Destroy();
		void Stop();
		virtual double UserProcess(double dTime);
		double GetTime();
	public:
		static vector<wstring> Enumerate();
		void SetUserFunction(double(*func)(double));
		double clip(double dSample, double dMax);
	private:
		double(*m_userFunction)(double);
		unsigned int m_nSampleRate;
		unsigned int m_nChannels;
		unsigned int m_nBlockCount;
		unsigned int m_nBlockCurrent;

		WAVEHDR *m_pWaveHeaders;
		HWAVEOUT m_hwDevice;

		T* m_pBlockMemory;
		thread m_thread;
		atomic<bool> m_bReady;
		atomic<unsigned int> m_nBlockFree;
		condition_variable m_cvBlockNotZero;
		mutex m_muxBlockNotZero;
		atomic<double> m_dGlobalTime;

		void waveOutProc(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwParam1, DWORD dwParam2);
		static void CALLBACK waveOutProcWrap(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwParam1, DWORD dwParam2);
		void MainThread();

};