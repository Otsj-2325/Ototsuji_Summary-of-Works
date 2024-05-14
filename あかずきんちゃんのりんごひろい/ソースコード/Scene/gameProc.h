#pragma once
#include<iostream>
#include"numDisp.h"
#include"sound.h"
#include"ground.h"
#include"player.h"
#include"item.h"
#include"enemy.h"
#include"goal.h"

#define NUN_ITEMS 5
#define SCORE_ITEM 10

class GMANAGER {
private:
	PTEX background;
	GROUND ground;
	PC* player;
	GOAL goal;
	ITEM items[NUN_ITEMS];
	ENEMY* emy;
	PTEX* trees[10];
	PTEX UI1;
	NUMDISPLAY ndsystem;

	bool useFlags[2] = { false };
	bool newEmyf = false;
	int flameCnt = 0;
	int score = 0;
	bool emyf = true;
	bool gameClearf=false;


public:
	float* groundLine;

	void gameInit();
	void gameUpdate();
	void gameDraw();
	void gameUninit();
	PTEX* getBullet() { return emy->getBullet(); }
	bool requestSceneChange();
	int getScore() { return score; }
	bool getClearf(){ return gameClearf; }


};