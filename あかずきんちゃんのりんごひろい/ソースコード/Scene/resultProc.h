#pragma once
#include"texPolygon.h"
#include"numDisp.h"

class RMANAGER {
private:
	bool plessButtonf=false;
	PTEX background;
	PTEX clearLogo;
	PTEX itemIcon;
	PTEX crossIcon;
	NUMDISPLAY ndsystem;
	bool clearf=false;
	int score = 0;

public:
	void setClearf(bool f){ clearf=f; }
	void resultInit();
	void resultUpdate();
	void resultDraw();
	void resultUninit();
	bool requestSceneChange();
	void setScore(int s) { score = s; }
};