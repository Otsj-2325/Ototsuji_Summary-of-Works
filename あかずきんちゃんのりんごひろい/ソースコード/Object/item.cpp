#include"item.h"

void ITEM::initData(char * tp, Float2 p, Float2 w, Float3 u, Float3 v)
{
	PTEX::initData(tp, p, w, u, v);

	for (int i = 0; i < ITEMLINE_NUM; i++) {
		fallPosHorizontal[i] = SCREEN_LEFT + (SCREEN_WIDTH / 10 * i + 80.0f);
	}


}

void ITEM::updateData()
{
	if (!usef) {
		if (getLimitRand(100) % 9 != 0) {
			usef = false;
			return;
		}
		else {
			do {
				fallPosNum = getLimitRand(8);
				usef = true;
				texset.pos.x = fallPosHorizontal[fallPosNum];
				texset.pos.y = SCREEN_UP + 100;
			} while (!usef);
		}
	}

	texset.pos.y += gravity;

	if (texset.pos.y - texset.wh.y > SCREEN_HEIGHT) {
		usef = false;

	}
}

void ITEM::drawData()
{
	PTEX::drawData();

}