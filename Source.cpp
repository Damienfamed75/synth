#include <iostream>
#include "olcNoiseMaker.h"
using namespace std;

atomic<double> FreqOutput = 0.0;

double MakeNoise(double dTime) {
	double SynthOutput = 1.0 * sin(FreqOutput * 2 * 3.14159 * dTime);

	return SynthOutput * 0.4;

	/*
	if (SynthOutput > 0.0)
		return 0.2;
	else
		return -0.2;*/
}

int main() {
	vector<wstring> devices = olcNoiseMaker<short>::Enumerate();

	for (auto d : devices) wcout << "Found Output Device:" << d << endl;
	
	olcNoiseMaker<short> sound(devices[0], 44100, 1, 8, 512);
	sound.SetUserFunction(MakeNoise);
	
	double OctaveBaseFreq = 110.0; // A2
	double OctaveRootModifier = pow(2.0, 1.0 / 12.0);
	
	while (true) {

		bool bKeyPressed = false;
		for (int i = 0; i < 15; i++) {
			if (GetAsyncKeyState((unsigned char)("ZSXDCFVGBHNJMK\xbcL\xbe"[i])) & 0x8000) {
				FreqOutput = OctaveBaseFreq * pow(OctaveRootModifier, i);
				bKeyPressed = true;
			}
		}
		if (!bKeyPressed)
			FreqOutput = 0.0;
	}
	
	return 0;
}