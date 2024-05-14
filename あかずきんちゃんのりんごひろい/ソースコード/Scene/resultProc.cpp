#include "resultProc.h"

void RMANAGER::resultInit()
{
	char* logoTex;

    if(clearf){
        logoTex=(char*)"data/TEXTURE/clear2.png";
    }
    else{
		score /= 2;
        logoTex= (char*)"data/TEXTURE/gover2.png";
    }

    background.initData((char*)"data/TEXTURE/sky.png", {0.0f, 0.0f}, {SCREEN_WIDTH,SCREEN_HEIGHT}, {0.0f, 1.0f, 0.0f},{0.0f, 1.0f, 0.0f});
    clearLogo.initData(logoTex, {0.0f, -150.0f}, {62.0f* 7 * 2, 64.0f * 2}, {0.0f, 1.0f, 0.0f},{0.0f, 1.0f, 0.0f});
    itemIcon.initData((char*)"data/TEXTURE/apple.png", {-180.0f, 50.0f}, {120.0f,120.0f}, {0.0f, 1.0f, 0.0f},{0.0f, 1.0f, 0.0f});
    crossIcon.initData((char*)"data/TEXTURE/cross2.png", {-64.5f, 50.0f}, {80.0f,80.0f}, {0.0f, 1.0f, 0.0f},{0.0f, 1.0f, 0.0f});
	ndsystem.initNumDips();

}

void RMANAGER::resultUpdate()
{
}

void RMANAGER::resultDraw()
{
    background.drawData();
    clearLogo.drawData();
    itemIcon.drawData();
    crossIcon.drawData();
	ndsystem.dispInt(score, 4, { 32.0f, 50.0f }); 
	ndsystem.setSize({75.0f, 100.0f});
}

void RMANAGER::resultUninit()
{
	ndsystem.unInitNumDips();
    background.unInitData();
    clearLogo.unInitData();
    itemIcon.unInitData();
    crossIcon.unInitData();
}

bool RMANAGER::requestSceneChange()
{
    if(GetKeyboardPress(DIK_SPACE)) {
		return true;
    }
    
    return false;
}
