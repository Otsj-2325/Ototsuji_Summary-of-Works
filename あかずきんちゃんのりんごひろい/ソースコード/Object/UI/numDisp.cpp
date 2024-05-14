#include "numDisp.h"
#include<iostream>
using namespace std;

void NUMDISPLAY::initNumDips()
{
	for (int i = 0; i < 10; i++) {
		numbers[i].initData((char*)"data/TEXTURE/numbers2.png", { 0.0f, 0.0f }, numbers[i].wh, { 0.0f, 1.0f / 5.0f, 0.0f }, { 0.0f, 1.0f / 10.0f - 0.05, 0.0f });
	}
}

void NUMDISPLAY::unInitNumDips()
{
	for (int i = 0; i < 10; i++) {
		numbers[i].unInitData();
	}
}

void NUMDISPLAY::dispInt(int data, int limit, Float2 p)
{
	int bits[9] = {0};
	int rem = (int)std::pow(10, limit-1);

	for (int i = limit - 1; i > -1; i--) {
		bits[i] = data / rem;
		data %= rem;
		rem /= 10;

		numbers[i].setNum(bits[i]);
		numbers[i].setPos(p);
		numbers[i].updateData();
		numbers[i].drawData();
		p.x += numbers[i].getWh().x;
	}


}

void NUMDISPLAY::setSize(Float2 w)
{
	for (int i = 0; i < 10; i++) {
		numbers[i].setWh(w);
	}
}

void NUM::updateData()
{
	setUV({ 1.0f / 10 * (num), 1.0f / 10 * (1 + num), 0.0f }, { 0.0f, 1.0f, 0.0f });
}
