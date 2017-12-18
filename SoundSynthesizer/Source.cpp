#include <iostream>
#include "olcNoiseMaker.h"
using namespace std;

atomic<double> FreqOutput = 0.0;

double MakeNoise(double dTime) {
	double SynthOutput = 1.0 * sin(FreqOutput * 2 * 3.14159 * dTime);

	return SynthOutput * 0.4; // sin waves

	/*
	if (SynthOutput > 0.0)
		return 0.2;
	else
		return -0.2;
	*/ // uncomment this out for square waves
}

int main() {
	vector<wstring> devices = olcNoiseMaker<short>::Enumerate();

	for (auto d : devices) wcout << "Found Output Device:" << d << endl;
	
	olcNoiseMaker<short> sound(devices[0], 44100, 1, 8, 512);
	sound.SetUserFunction(MakeNoise);
	
	double OctaveBaseFreq = 220.0; // A2 is 110
	double OctaveRootModifier = pow(2.0, 1.0 / 12.0);
	
	while (true) {

		bool bKeyPressed = false;
		// Watch for spotty regex here!!
		for (int i = 0; i < 40; i++) {
			if (GetAsyncKeyState((unsigned char)("QAWZSEXDRCFTVGYBHUNJIMKOLP1234567890\xbc\xbe\b\\"[i]/*this is some spotty regex*/)) & 0x8000 && bKeyPressed == false) {
				FreqOutput = OctaveBaseFreq * pow(OctaveRootModifier, i) + 10;
				bKeyPressed = true;
			}
		}
		if (GetAsyncKeyState(VK_SPACE) && bKeyPressed == false) { // Spacebar
			FreqOutput = OctaveBaseFreq * pow(OctaveRootModifier, 28);
			bKeyPressed = true;
		}
		if (GetAsyncKeyState(VK_RETURN) && bKeyPressed == false) { // Enter
			FreqOutput = OctaveBaseFreq * pow(OctaveRootModifier, 30);
			bKeyPressed = true;
		}
		if (GetAsyncKeyState(VK_SHIFT) && bKeyPressed == false) { // Left and Right Shift
			FreqOutput = OctaveBaseFreq * pow(OctaveRootModifier, 29);
			bKeyPressed = true;
		}
		if (GetAsyncKeyState(VK_CONTROL) && bKeyPressed == false) { // Left and Right Control
			FreqOutput = OctaveBaseFreq * pow(OctaveRootModifier, 32);
			bKeyPressed = true;
		}
		if (GetAsyncKeyState(VK_OEM_MINUS) && bKeyPressed == false) { // - and _
			FreqOutput = OctaveBaseFreq * pow(OctaveRootModifier, 33);
			bKeyPressed = true;
		}
		if (GetAsyncKeyState(VK_OEM_PLUS) && bKeyPressed == false) { // = and +
			FreqOutput = OctaveBaseFreq * pow(OctaveRootModifier, 34);
			bKeyPressed = true;
		}
		if (GetAsyncKeyState(VK_TAB) && bKeyPressed == false) { // Tab
			FreqOutput = OctaveBaseFreq * pow(OctaveRootModifier, 37);
			bKeyPressed = true;
		}

		if (!bKeyPressed)
			FreqOutput = 0.0;
	}
	
	return 0;
}