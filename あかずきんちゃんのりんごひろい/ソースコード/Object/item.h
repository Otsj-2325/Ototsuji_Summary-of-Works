#pragma once
#include"texPolygon.h"
#include"randam.h"

#define ITEMLINE_NUM 10

class ITEM:public PTEX {
private:
	float fallPosHorizontal[ITEMLINE_NUM];
	int fallPosNum;
	float gravity = 3.0f;


public:
	bool usef = false;

	void initData(char* tp, Float2 p, Float2 w, Float3 u, Float3 v) override;
	void updateData() override;
	void drawData() override;
};