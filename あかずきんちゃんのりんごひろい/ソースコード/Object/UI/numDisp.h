#pragma once
#include"texPolygon.h"
#include"main.h"
#include<cmath>

class NUM :public PTEX {
private:
	int num;

public:
	Float2 wh = { 45.0f, 64.0f };

	void setUV(Float3 u, Float3 v) { texset.u = u; texset.v = v; }
	void setPos(Float2 p) { texset.pos = p; };
	void setWh(Float2 w) { texset.wh = w; };
	void setNum(int n) { num = n; };
	void updateData() override;

};

class NUMDISPLAY {
private:
	NUM numbers[10];


public:
	void initNumDips();
	void unInitNumDips();
	void dispInt(int data, int limit, Float2 p);
	void setSize(Float2 w);

};