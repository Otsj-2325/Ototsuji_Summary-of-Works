#include "scaneManager.h"

void SMANAGER::sceneManagerInit(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	InitRenderer(hInstance, hWnd, bWindow);
	InitInput(hInstance, hWnd);
	InitSound(hWnd);
	initRand();
}

void SMANAGER::sceneManagerUninit()
{
	UninitTexture();
	UninitSound();
	UninitInput();
	UninitRenderer();
}

void SMANAGER::sceneChange()
{

	if (!changef) {
		return;
	}

	frameCnt = 0;

	if (nScene == SCENE::NSCENE) {
		if (!firstf) {
			/*result Uninit*/
			//rmgr->resultUninit();

		}

		/*nScene = TITLE*/
		nScene = SCENE::TITLE;
		/*title Inint*/		
		tmgr = new TMANAGER();
		tmgr->titleInit();
		SoundCtrl(0, true, 1);
	}
	else if (nScene == SCENE::TITLE) {
		/*title Uninit*/
		SoundCtrl(0, false, 0);
		tmgr->titleUninit();
		delete tmgr;
		/*nScene == GAME*/
		nScene = SCENE::GAME;
		/*game Init*/
		gmgr=new GMANAGER();
		gmgr->gameInit();
		SoundCtrl(1, true, 0);

	}
	else if (nScene == SCENE::GAME) {
		/*game Uninit*/
		int score = gmgr->getScore();
		gameClearf=gmgr->getClearf();
		SoundCtrl(1, false, 0);
		gmgr->gameUninit();
		delete gmgr;
		/*nScene == RESULT*/
		nScene = SCENE::RESULT;
		/*result Init*/
		rmgr=new RMANAGER();
		rmgr->setScore(score);
		rmgr->setClearf(gameClearf);
		rmgr->resultInit();
		SoundCtrl(2, true, 0);
	}
	else if (nScene == SCENE::RESULT) {
		/*result Uninit*/
		SoundCtrl(2, false, 0);
		rmgr->resultUninit();
		delete rmgr;
		firstf = false;
		/*nScene == NULL*/
		nScene = SCENE::NSCENE;
	}
}

void SMANAGER::sceneUpdate()
{
	changef = true;

	if (nScene == SCENE::NSCENE) {
		sceneChange();
		changef = false;
	}
	else if (nScene == SCENE::TITLE) {
		UpdateInput();
		tmgr->titleUpdate();
		if (tmgr->requestSceneChange()) {
			sceneChange();
			changef = false;
			frameCnt = 0;
		}
	}
	else if (nScene == SCENE::GAME) {
		UpdateInput();
		gmgr->gameUpdate();
		if (gmgr->requestSceneChange()) {
			sceneChange();
			changef = false;
			frameCnt = 0;
		}
	}
	else if (nScene == SCENE::RESULT) {
		UpdateInput();
		rmgr->resultUpdate();
		if (rmgr->requestSceneChange()) {
			sceneChange();
			frameCnt = 0;
		}
	}
	
#ifdef _DEBUG
	std::cout << "changef is" << changef << std::endl;
#endif // _DEBUG

}

void SMANAGER::sceneDraw()
{
	Clear();
	SetWorldViewProjection2D();
	SetDepthEnable(false);

	if (nScene == SCENE::NSCENE) {
	}
	else if (nScene == SCENE::TITLE) {
		tmgr->titleDraw();
	}
	else if (nScene == SCENE::GAME) {
		gmgr->gameDraw();
	}
	else if (nScene == SCENE::RESULT) {
		rmgr->resultDraw();
	}
	Present();
}