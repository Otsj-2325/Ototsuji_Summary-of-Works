#include "titleProc.h"
/*CURSOR*/
void CURSOR::updateData(){
	if(GetKeyboardPress(DIK_SPACE)) {
		if (positionf) {
			plessButtonf = true;
			//PlaySE(button);
		}
		else {
			exit(1);
		}
	}

	if(GetKeyboardPress(DIK_A)) {
		positionf=true;
	}
	
	if (GetKeyboardPress(DIK_D) ) {
		positionf = false;
	}

	if(positionf){
		texset.pos.x = cursorPos1;

	}
	else{
		texset.pos.x = cursorPos2;
	}
}

/*TITLE #pragma*/
void TMANAGER::titleInit()
{
	background.initData((char*)"data/TEXTURE/sky.png", bgPosition, bgWh, bgU, bgV);
	titleLogo.initData((char*)"data/TEXTURE/appleTitle2.png", logoPosition, logoWh, logoU, logoV);
	startButton.initData((char*)"data/TEXTURE/button2.png", {-SCREEN_WIDTH / 4 + 100, SCREEN_HEIGHT / 3}, {96.0f * 4, 96.0f }, {0.0f, 1.0f, 0.0f},{0.0f, 0.5f, 0.0f});
	exitButton.initData((char*)"data/TEXTURE/button2.png", { SCREEN_WIDTH / 4 + 100, SCREEN_HEIGHT / 3 }, { 96.0f * 4, 96.0f }, {0.0f, 1.0f, 0.0f},{0.5f, 1.0f, 0.0f});
	cursor.initData((char*)"data/TEXTURE/apple.png", {0.0f, SCREEN_HEIGHT / 3 }, {160.0f / 1.5f, 160.0f / 1.5f}, {0.0f, 1.0f, 0.0f},{0.0f, 1.0f, 0.0f});

}

void TMANAGER::titleUpdate()
{
	cursor.updateData();
}

void TMANAGER::titleDraw()
{
	background.drawData();
	titleLogo.drawData();
	startButton.drawData();
	exitButton.drawData();
	cursor.drawData();

}

void TMANAGER::titleUninit()
{
	background.unInitData();
	titleLogo.unInitData();
	startButton.unInitData();
	exitButton.unInitData();
	cursor.unInitData();

}

bool TMANAGER::requestSceneChange()
{
	if (cursor.getPlessButton()) {
		return true;
	}

	return false;
}
