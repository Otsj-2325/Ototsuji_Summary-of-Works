#pragma once
#include"titleProc.h"
#include"gameProc.h"
#include"resultProc.h"
#include"gameMusic.h"

#include"renderer.h"
#include"input.h"
#include"sound.h"
#include"texture.h"

enum class SCENE{
	NSCENE,
	TITLE,
	GAME,
	RESULT,
};

class SMANAGER {
private:
	TMANAGER* tmgr;
	GMANAGER* gmgr;
	RMANAGER* rmgr;
	SCENE nScene = SCENE::NSCENE;
	bool firstf = true;
	bool changef = true;
	int frameCnt = 0;
	bool gameClearf = false;

public:
	void sceneManagerInit(HINSTANCE hInsrance, HWND hWnd, BOOL bWindow);
	void sceneManagerUninit();
	void sceneChange();

	void sceneUpdate();
	void sceneDraw();

};