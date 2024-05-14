#pragma once
#include"sound.h"

void SoundCtrl(int handle, bool ctrl_f, int loopCnt);


class SOUNDSYSTEM {
typedef struct {
	int handle;
	bool ctrlf;
}SOUNDDATA;

private:
	const char bgmpass[10][50];
	SOUNDDATA sDatas[10];
	int soundNum;
	float soundVol = 0.01f;

public:
	void soundController(int soundNum, bool ctrlf, int loopCnt);
	void setSoundVolume(float vol);
};