#ifdef _DEBUG
#include<iostream>
#endif

#include"ground.h"

void GROUND::initData(char * tp, Float2 p, Float2 w, Float3 u, Float3 v)
{
	PTEX::initData(tp, p, w, u, v);
	groundLine = texset.pos.y - texset.wh.y / 2;
}