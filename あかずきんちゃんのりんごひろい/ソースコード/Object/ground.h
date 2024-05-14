#pragma once
#include"texPolygon.h"

class GROUND:public PTEX {
private:
	float groundLine;

public:
	float* getGroundLine() { return &groundLine; }
	void initData(char * tp, Float2 p, Float2 w, Float3 u, Float3 v) override;
};