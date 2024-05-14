#pragma once
#include"texPolygon.h"
#include"numDisp.h"

class CURSOR :public PTEX{
private:
	bool plessButtonf = false;
	bool positionf = true;

	float cursorPos1 = -380.0f;
	float cursorPos2 = 100.0f;

public:
	void updateData() override;
	bool getPlessButton(){return plessButtonf;}
};

class TMANAGER {
private:
	PTEX background;
	const Float2 bgPosition = { 0.0f, 0.0f };
	const Float2 bgWh = { SCREEN_WIDTH, SCREEN_HEIGHT };
	const Float3 bgU = { 0.0f, 1.0f, 0.0f };
	const Float3 bgV = { 0.0f, 1.0f, 0.0f };

	PTEX titleLogo;
	const Float2 logoPosition = { 0.0f, -SCREEN_HEIGHT / 6 };
	const Float2 logoWh = { 960.0f, 216.0f };
	const Float3 logoU = { 0.0f, 1.0f, 0.0f };
	const Float3 logoV = { 0.0f, 1.0f, 0.0f };

	PTEX startButton;
	const Float2 sButtonPosition = { -SCREEN_WIDTH / 4 + 100, SCREEN_HEIGHT / 3 };
	const Float2 sButtonWh = { 96.0f * 4, 96.0f };
	const Float3 sButtonU = { 0.0f, 1.0f, 0.0f };
	const Float3 sButtonV = { 0.0f, 0.5f, 0.0f };
	
	PTEX exitButton;
	const Float2 eButtonPosition = { SCREEN_WIDTH / 4 + 100, SCREEN_HEIGHT / 3 };
	const Float2 eButtonWh = { 96.0f * 4, 96.0f };
	const Float3 eButtonU = { 0.0f, 0.5f, 0.0f };
	const Float3 eButtonV = { 0.0f, 1.0f, 0.0f };

	CURSOR cursor;
	const Float2 cursorPosition = { 0.0f, SCREEN_HEIGHT / 3 };
	const Float2 cursorWh = { 160.0f / 1.5f, 160.0f / 1.5f };
	const Float3 cursorU = { 0.0f, 1.0f, 0.0f };
	const Float3 cursorV = { 0.0f, 1.0f, 0.0f };

	NUMDISPLAY nds;



public:
	void titleInit();
	void titleUpdate();
	void titleDraw();
	void titleUninit();
	bool requestSceneChange();
};