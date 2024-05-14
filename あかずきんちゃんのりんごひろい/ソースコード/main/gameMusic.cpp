#include"gameMusic.h"

/// <param name="handle">‚O”Ô–Ú‚©‚ç”‚¦‚Ä‰½”Ô–Ú‚©</param>
/// <param name="ctrl_f">‚P‚ÅÄ¶@‚Q‚Å’â~</param>
/// <param name="loopCnt">‚P‚Åƒ‹[ƒvÄ¶@‚O‚Å”ñƒ‹[ƒvÄ¶</param>
void SoundCtrl(int handle, bool ctrl_f, int loopCnt) {
	static char bgmpass[10][50] = {
		"data\\BGM201.wav",
		"data\\BGM202.wav",
		"data\\BGM203.wav",

	};
	static int handles[3][10] = {
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0
	};

	float soundVol = 0.02f;

	if (ctrl_f) {
		handles[0][handle] = LoadSound(bgmpass[handle]);
		handles[2][handle] = loopCnt;
		PlaySound(handles[0][handle], handles[2][handle]);
		SetVolume(handles[0][handle], soundVol);
		handles[1][handle] = 1;
	}
	else {
		if (handles[1][handle] == 1) {
			StopSound(handles[0][handle]);
			handles[1][handle] = 0;
		}
	}
}

void SOUNDSYSTEM::soundController(int soundNum, bool ctrlf, int loopCnt) {
	if (ctrlf) {
		sDatas[soundNum].handle = LoadSound((char*)bgmpass[soundNum]);
		PlaySound(sDatas[soundNum].handle, loopCnt);
		SetVolume(sDatas[soundNum].handle, soundVol);
		sDatas[soundNum].ctrlf = true;

	}
	else {
		if (sDatas[soundNum].ctrlf) {
			StopSound(sDatas[soundNum].handle);
			sDatas[soundNum] = { -10, false };
		}
	}
}

void SOUNDSYSTEM::setSoundVolume(float vol)
{
	

}
